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

key_t       alita::lqueue::KEY = 99029;
std::size_t alita::lqueue::SIZE = 2000000;
std::size_t alita::lqueue::MIN_SIZE = 2000;
std::size_t alita::lqueue::MAX_SIZE = 20000000000;

void alita::lqueue::set_size(std::size_t size)
{
    if(size < alita::lqueue::MIN_SIZE || size > alita::lqueue::MAX_SIZE)
        throw std::runtime_error("Invalid Size...");

    lqueue::SIZE = size;
}

alita::lqueue& alita::lqueue::instance()
{
    static alita::lqueue lqueue(alita::lqueue::SIZE);
    return lqueue;
}

alita::lqueue::lqueue() : alita::lqueue::lqueue(alita::lqueue::SIZE)
{    
}

alita::lqueue::lqueue(std::size_t size)
{
    if(size < alita::lqueue::MIN_SIZE || size > alita::lqueue::MAX_SIZE)
        throw std::runtime_error("Invalid Size...");

    this->_queue_size = size;
    this->_queue_count = size / sizeof(alita::queue_item);

    this-> _shm_id = shmget(alita::lqueue::KEY, size + sizeof(alita::header),  IPC_CREAT | IPC_EXCL | 0660);
    if(this-> _shm_id < 0) {
        std::cerr << "INFO : SHM EXISTS" << std::endl;
        this-> _shm_id = shmget(alita::lqueue::KEY, size + sizeof(alita::header), IPC_CREAT | 0660);
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

    this->init_mutex(&h._sync_context._lock, &h._sync_context._lock_attr);

    this->init_cv(&h._sync_context._can_be_enqueued, &h._sync_context._can_be_enqueued_attr);
    this->init_cv(&h._sync_context._can_be_dequeued, &h._sync_context._can_be_dequeued_attr);
    
    memcpy(this->_shm_ptr, &h, sizeof(header));
}

void alita::lqueue::init_mutex(pthread_mutex_t* mutex, pthread_mutexattr_t* mutex_attr)
{
    this->validate(pthread_mutexattr_init(mutex_attr), "MUTEX ATTR INIT ERROR");
    this->validate(pthread_mutexattr_setpshared(mutex_attr, PTHREAD_PROCESS_SHARED), "MUTEX SET SHARED ERROR");
    this->validate(pthread_mutex_init(mutex, mutex_attr), "MUTEX INIT ERROR");
}

void alita::lqueue::desroy_mutex(pthread_mutex_t* mutex, pthread_mutexattr_t* mutex_attr)
{
    this->validate(pthread_mutex_destroy(mutex), "MUTEX DESTROY ERROR");    
    this->validate(pthread_mutexattr_destroy(mutex_attr), "MUTEX ATTR DESTROY ERROR");   
}

void alita::lqueue::init_cv(pthread_cond_t* cv, pthread_condattr_t* cv_attr)
{
    this->validate(pthread_condattr_init(cv_attr), "CV ATTR INIT ERROR");
    this->validate(pthread_condattr_setpshared(cv_attr, PTHREAD_PROCESS_SHARED), "CV SET SHARED ERROR");
    this->validate(pthread_cond_init(cv, cv_attr), "CV INIT ERROR");
}

void alita::lqueue::destroy_cv(pthread_cond_t* cv, pthread_condattr_t* cv_attr)
{
    this->validate(pthread_cond_destroy(cv), "CV DESTROY ERROR");
    this->validate(pthread_condattr_destroy(cv_attr), "CV ATTR DESTROY ERROR");
}

void alita::lqueue::validate(int err, std::string message)
{
    if(err != 0)
        throw std::runtime_error(message + " : " + std::string(strerror(err)));
}

void alita::lqueue::unsync()
{
    alita::header* header = (alita::header*)this->_shm_ptr;
    
    this->desroy_mutex(&header->_sync_context._lock, &header->_sync_context._lock_attr);

    this->destroy_cv(&header->_sync_context._can_be_enqueued, &header->_sync_context._can_be_enqueued_attr);
    this->destroy_cv(&header->_sync_context._can_be_dequeued, &header->_sync_context._can_be_dequeued_attr);
}

void alita::lqueue::detach()
{
    if(shmdt(this->_shm_ptr) != 0)
        throw std::runtime_error(strerror(errno));
}

void alita::lqueue::destroy()
{
    if(shmctl(this->_shm_id, IPC_RMID, NULL) != 0)
        throw std::runtime_error(strerror(errno));
}

void alita::lqueue::dump()
{
    alita::header* header = (alita::header*)this->_shm_ptr;
    alita::queue_item* items = (alita::queue_item*)((char*)this->_shm_ptr + header->_base_offset);

    this->validate(pthread_mutex_lock(&header->_sync_context._lock), "MUTEX LOCK ERROR");

    std::cerr << "STATE   : " << header->_state << std::endl
              << "SIZE    : " << header->_size << std::endl
              << "COUNT   : " << this->_queue_count << std::endl
              << "H_COUNT : " << header->_count << std::endl
              << "OFFSET  : " << header->_base_offset << std::endl
              << "FRONT   : " << header->_front << std::endl
              << "REAR    : " << header->_rear << std::endl; 

    if(header->_front == -1) {
        std::cerr << "QUEUE IS EMPTY!!" << std::endl;
        this->validate(pthread_mutex_unlock(&header->_sync_context._lock), "MUTEX UNLOCK ERROR");
        return;
    }

    std::cerr << "QUEUE CONTENT : " << std::endl;
    if(header->_rear >= header->_front) {
        for(int i = header->_front; i <= header->_rear; i++)
            std::cerr << "LINK " << i << " : " << items[i]._link._link << std::endl;
        
        this->validate(pthread_mutex_unlock(&header->_sync_context._lock), "MUTEX UNLOCK ERROR");

        return;
    }

    for(int i = header->_front; i < this->_queue_count; i++)
        std::cerr << "LINK " << i << " : "<< items[i]._link._link << std::endl;

    for(int i = 0; i < header->_rear; i++)
        std::cerr << "LINK " << i << " : " << items[i]._link._link << std::endl;
        
    this->validate(pthread_mutex_unlock(&header->_sync_context._lock), "MUTEX UNLOCK ERROR");
}

void alita::lqueue::enqueue(const std::string link)
{
    this->enqueue(link.c_str());
}

void alita::lqueue::enqueue(const char* link)
{
    if(link == NULL)
        throw std::invalid_argument("Link is NULL...");
    if(strlen(link) > 2000)
        throw std::invalid_argument("Link is too long...");

    alita::queue_item qi;
    strcpy(qi._link._link, link);

    this->enqueue(&qi);
}

void alita::lqueue::enqueue(const alita::queue_item* queue_item)
{
    if(queue_item == NULL)
        throw std::invalid_argument("Invalid queue item...");

    alita::header* header = (alita::header*)this->_shm_ptr;
    alita::queue_item* items = (alita::queue_item*)((char*)this->_shm_ptr + header->_base_offset);
    this->validate(pthread_mutex_lock(&header->_sync_context._lock), "MUTEX LOCK ERROR");
    
    bool condition = false;
    while(true) {
        condition = (header->_front == 0 && header->_rear == this->_queue_count - 1) || 
			        (header->_rear == (header->_front - 1) % (this->_queue_count -1 )); 

        if(!condition)
            break;

        pthread_cond_wait(&header->_sync_context._can_be_enqueued, &header->_sync_context._lock);
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

    pthread_cond_signal(&header->_sync_context._can_be_dequeued);
    this->validate(pthread_mutex_unlock(&header->_sync_context._lock), "MUTEX UNLOCK ERROR");
}

void alita::lqueue::dequeue(alita::queue_item* queue_item)
{
    if(queue_item == NULL)
        throw std::invalid_argument("Output param is NULL...");

    alita::header* header = (alita::header*)this->_shm_ptr;
    alita::queue_item* items = (alita::queue_item*)((char*)this->_shm_ptr + header->_base_offset);
    this->validate(pthread_mutex_lock(&header->_sync_context._lock), "MUTEX LOCK ERROR");

    while(header->_front == -1)
        pthread_cond_wait(&header->_sync_context._can_be_dequeued, &header->_sync_context._lock);

    header->_count--;
    memcpy(queue_item, &items[header->_front], sizeof(alita::queue_item));

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

    pthread_cond_signal(&header->_sync_context._can_be_enqueued);
    this->validate(pthread_mutex_unlock(&header->_sync_context._lock), "MUTEX UNLOCK ERROR");
}