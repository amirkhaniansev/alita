/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Implementation of thread pool library.
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
 * Full notice : https://github.com/amirkhaniansev/alita/tree/master/LICENSE
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
**/

#include "../include/threadpool.hpp"

#include <cstring>
#include <iostream>

alita::threadpool& alita::threadpool::instance()
{
    static alita::threadpool instance;
    return instance;
}

alita::threadpool::threadpool()
{
    this->validate(pthread_mutex_init(&this->_works_lock, NULL));
    this->validate(pthread_mutex_init(&this->_workers_lock, NULL));
    this->validate(pthread_cond_init(&this->_can_be_dequeued,NULL));

    for(auto i = 0; i < 4; i++)
        this->create_worker(true);
}

void alita::threadpool::enqueue_work(entry_point_t entry_point, input_t input, callback_t callback)
{
    if(entry_point == nullptr)
        throw std::invalid_argument("INVALID ENTRY POINT");

    alita::execution_context context;
    context._entry_point = entry_point;
    context._callback = callback;
    context._input = input;

    this->validate(pthread_mutex_lock(&this->_works_lock));
    this->_works.push(context);

    if(this->_workers.size() <= 32UL)
        this->create_worker(false);

    pthread_cond_signal(&this->_can_be_dequeued);
    
    this->validate(pthread_mutex_unlock(&this->_works_lock));
}

void alita::threadpool::create_worker(bool is_main)
{
    pthread_t pthread_id;
    this->validate(pthread_create(&pthread_id, NULL, alita::threadpool::start_routine, (void*)is_main));
    this->validate(pthread_detach(pthread_id));

    this->validate(pthread_mutex_lock(&this->_workers_lock));
    this->_workers.insert(pthread_id);
    this->validate(pthread_mutex_unlock(&this->_workers_lock));
}

void alita::threadpool::do_work(bool is_main)
{
    do
    {
        this->validate(pthread_mutex_lock(&this->_works_lock));

        while(this->_works.empty())
            pthread_cond_wait(&this->_can_be_dequeued, &this->_works_lock);

        auto work = this->_works.front();
        this->_works.pop();
        this->busy_count++;

        this->validate(pthread_mutex_unlock(&this->_works_lock));

        auto output = work._entry_point(work._input);
        if(work._callback != nullptr)
            output = work._callback(work._input, output);
        
        this->validate(pthread_mutex_lock(&this->_works_lock));
        this->busy_count--;
        this->validate(pthread_mutex_unlock(&this->_works_lock));
    } while (is_main); 

    this->validate(pthread_mutex_lock(&this->_workers_lock));
    this->_workers.erase(pthread_self());
    this->validate(pthread_mutex_unlock(&this->_workers_lock));
}

void alita::threadpool::state()
{
    this->validate(pthread_mutex_lock(&this->_works_lock));
    this->validate(pthread_mutex_lock(&this->_workers_lock));

    std::string state = "BUSY COUNT :" + std::to_string(this->busy_count) + '\n'
              + "WORKS COUNT : "   + std::to_string(this->_works.size()) + '\n'
              + "WORKERS COUNT : " + std::to_string(this->_workers.size()) + '\n';
    std::cerr << state;

    this->validate(pthread_mutex_unlock(&this->_works_lock));
    this->validate(pthread_mutex_unlock(&this->_workers_lock));
}

void alita::threadpool::exit()
{
    pthread_exit(0);
}

void* alita::threadpool::start_routine(void* input)
{
    alita::threadpool::instance().do_work((bool)input);
}

void alita::threadpool::destroy()
{
    this->validate(pthread_mutex_destroy(&this->_works_lock));
    this->validate(pthread_mutex_destroy(&this->_workers_lock));
    this->validate(pthread_cond_destroy(&this->_can_be_dequeued));
}

void alita::threadpool::validate(int err)
{
    if(err != 0)
        throw std::runtime_error(strerror(err));
}