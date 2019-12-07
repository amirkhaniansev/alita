/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Implementation for links queue.
 * Copyright (C) <2019>
 *      Authors: <amirkhaniansev>  <amirkhanyan.sevak@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * Full notice : https://github.com/amirkhaniansev/search-engine/tree/master/LICENSE
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#include <stdexcept>
#include <cstring>
#include <iostream>

#include <sys/ipc.h>
#include <sys/shm.h>

#include "../include/lqueue.hpp"

key_t       se::lqueue::KEY = 99029;
std::size_t se::lqueue::SIZE = 2000000;
std::size_t se::lqueue::MIN_SIZE = 2000;
std::size_t se::lqueue::MAX_SIZE = 20000000000;

void se::lqueue::set_size(std::size_t size)
{
    if(size < se::lqueue::MIN_SIZE || size > se::lqueue::MAX_SIZE)
        throw std::runtime_error("Invalid Size...");

    lqueue::SIZE = size;
}

se::lqueue& se::lqueue::instance()
{
    static se::lqueue lqueue(se::lqueue::SIZE);
    return lqueue;
}

se::lqueue::lqueue() : se::lqueue::lqueue(se::lqueue::SIZE)
{    
}

se::lqueue::lqueue(std::size_t size)
{
    if(size < se::lqueue::MIN_SIZE || size > se::lqueue::MAX_SIZE)
        throw std::runtime_error("Invalid Size...");

    this->_queue_size = size;
    this->_queue_count = size / sizeof(se::queue_item);

    this-> _shm_id = shmget(se::lqueue::KEY, size + sizeof(se::header),  IPC_CREAT | IPC_EXCL | 0660);
    if(this-> _shm_id < 0) {
        std::cerr << "INFO : SHM EXISTS" << std::endl;
        this-> _shm_id = shmget(se::lqueue::KEY, size + sizeof(se::header), IPC_CREAT | 0660);
        if(this-> _shm_id < 0)
            throw std::runtime_error("SHM GET ERROR : " + std::string(strerror(errno)));
    
        this->_shm_ptr = shmat(this-> _shm_id, NULL, 0);
        if(this->_shm_ptr == (void*)-1)
            throw std::runtime_error("SHM ATTACH ERROR : " + std::string(strerror(errno)));

        return;
    } 

    this->_shm_ptr = shmat(this-> _shm_id, NULL, 0);
    if(this->_shm_ptr == (void*)-1)
        throw std::runtime_error(strerror(errno));

    header h;
    h._size = this->_queue_size;
    h._count = 0;
    h._front = -1;
    h._rear = -1;
    h._state = 'I';
    h._base_offset = sizeof(header);

    this->validate(pthread_mutexattr_init(&h._lock_attr), "MUTEX ATTR INIT ERROR");
    this->validate(pthread_mutexattr_setpshared(&h._lock_attr, PTHREAD_PROCESS_SHARED), "MUTEX SET SHARED ERROR");
    this->validate(pthread_mutex_init(&h._lock, &h._lock_attr), "MUTEX INIT ERROR");

    this->validate(pthread_condattr_init(&h._cv_attr), "CV ATTR INIT ERROR");
    this->validate(pthread_condattr_setpshared(&h._cv_attr, PTHREAD_PROCESS_SHARED), "CV SET SHARED ERROR");
    this->validate(pthread_cond_init(&h._cv, &h._cv_attr), "CV INIT ERROR");

    memcpy(this->_shm_ptr, &h, sizeof(header));
}

void se::lqueue::validate(int err, std::string message)
{
    if(err != 0)
        throw std::runtime_error(message + " : " + std::string(strerror(err)));
}

void se::lqueue::unsync()
{
    se::header* header = (se::header*)this->_shm_ptr;

    this->validate(pthread_mutex_destroy(&header->_lock), "MUTEX DESTROY ERROR");    
    this->validate(pthread_mutexattr_destroy(&header->_lock_attr), "MUTEX ATTR DESTROY ERROR");    

    this->validate(pthread_cond_destroy(&header->_cv), "CV DESTROY ERROR");
    this->validate(pthread_condattr_destroy(&header->_cv_attr), "CV ATTR DESTROY ERROR");
}

void se::lqueue::detach()
{
    if(shmdt(this->_shm_ptr) != 0)
        throw std::runtime_error(strerror(errno));
}

void se::lqueue::destroy()
{
    if(shmctl(this->_shm_id, IPC_RMID, NULL) != 0)
        throw std::runtime_error(strerror(errno));
}

void se::lqueue::dump()
{
    se::header* header = (se::header*)this->_shm_ptr;
    se::queue_item* items = (se::queue_item*)((char*)this->_shm_ptr + header->_base_offset);

    std::cerr << "STATE   : " << header->_state << std::endl
              << "SIZE    : " << header->_size << std::endl
              << "COUNT   : " << this->_queue_count << std::endl
              << "H_COUNT : " << header->_count << std::endl
              << "OFFSET  : " << header->_base_offset << std::endl
              << "FRONT   : " << header->_front << std::endl
              << "REAR    : " << header->_rear << std::endl; 

    if(header->_front == -1) {
        std::cerr << "QUEUE IS EMPTY!!" << std::endl;
        return;
    }

    std::cerr << "QUEUE CONTENT : " << std::endl;
    if(header->_rear >= header->_front) {
        for(int i = header->_front; i <= header->_rear; i++)
            std::cerr << "LINK : " << items[i]._link._link << std::endl;

        return;
    }

    for(int i = header->_front; i < this->_queue_count; i++)
        std::cerr << "LINK : " << items[i]._link._link << std::endl;

    for(int i = 0; i < header->_rear; i++)
        std::cerr << "LINK : " << items[i]._link._link << std::endl;
}

void se::lqueue::enqueue(const std::string link)
{
    this->enqueue(link.c_str());
}

void se::lqueue::enqueue(const char* link)
{
    if(link == NULL)
        throw std::invalid_argument("Link is NULL...");
    if(strlen(link) > 2000)
        throw std::invalid_argument("Link is too long...");

    se::queue_item qi;
    strcpy(qi._link._link, link);

    this->enqueue(&qi);
}

void se::lqueue::enqueue(const se::queue_item* queue_item)
{
    if(queue_item == NULL)
        throw std::invalid_argument("Invalid queue item...");

    se::header* header = (se::header*)this->_shm_ptr;
    se::queue_item* items = (se::queue_item*)((char*)this->_shm_ptr + header->_base_offset);
    this->validate(pthread_mutex_lock(&header->_lock), "MUTEX LOCK ERROR");
    
    bool condition = false;
    while(true) {
        condition = (header->_front == 0 && header->_rear == this->_queue_count - 1) || 
			        (header->_rear == (header->_front - 1) % (this->_queue_count -1 )); 

        if(!condition)
            break;

        pthread_cond_wait(&header->_cv, &header->_lock);
    }
    
    if(header->_front == -1){
        header->_front = 0;
        header->_rear = 0;
    }
    else if (header->_rear == this->_queue_count - 1 && header->_front != 0) {
        header->_rear = 0;
    }   
    else {
        header->_rear++;
    }

    header->_count++;
    items[header->_rear] = *queue_item;

    pthread_cond_signal(&header->_cv);
    this->validate(pthread_mutex_unlock(&header->_lock), "MUTEX UNLOCK ERROR");
}

void se::lqueue::dequeue(se::queue_item* queue_item)
{
    if(queue_item == NULL)
        throw std::invalid_argument("Output param is NULL...");

    se::header* header = (se::header*)this->_shm_ptr;
    se::queue_item* items = (se::queue_item*)((char*)this->_shm_ptr + header->_base_offset);
    this->validate(pthread_mutex_lock(&header->_lock), "MUTEX LOCK ERROR");

    while(header->_front == -1)
        pthread_cond_wait(&header->_cv, &header->_lock);

    header->_count--;
    memcpy(queue_item, &items[header->_front], sizeof(se::queue_item));

    if(header->_front == header->_rear) {
        header->_front = -1;
        header->_rear = -1;
    }
    else if (header->_front == this->_queue_count - 1) {
        header->_front = 0;
    }
    else {
        header->_front++;
    }

    pthread_cond_signal(&header->_cv);
    this->validate(pthread_mutex_unlock(&header->_lock), "MUTEX UNLOCK ERROR");
}