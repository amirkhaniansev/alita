/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Header file for links queue.
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

#ifndef __L_QUEUE_HPP__
#define __L_QUEUE_HPP__

#include <cstddef>
#include <string>

#include <pthread.h>
#include <sys/ipc.h>

namespace se {
    struct link {
        char _link[2000];
    };

    struct queue_item {
        link _link;
    };

    struct header {
        char _state;
        pthread_mutex_t _lock;
        pthread_mutexattr_t _lock_attr;
        pthread_cond_t _cv;
        pthread_condattr_t _cv_attr;
        int _front;
        int _rear;
        std::size_t _count;
        std::size_t _size;
        std::size_t _base_offset;
    };

    class lqueue {
        public:
            static void set_size(std::size_t size);
            static lqueue& instance();
            lqueue(lqueue const&)          = delete;
            void operator=(lqueue const&)  = delete;
            void enqueue(const char* link);
            void enqueue(const std::string link);
            void enqueue(const queue_item* queue_item);
            void dequeue(queue_item* queue_item);
            void dump();
            void detach();
            void unsync();
            void destroy();
        protected:
            lqueue();
            lqueue(std::size_t queue_size);
            void validate(int err, std::string message);
        private:
            static key_t KEY;
            static std::size_t SIZE;
            static std::size_t MIN_SIZE;
            static std::size_t MAX_SIZE;
            std::size_t _queue_size;
            int _queue_count;
            int _shm_id;
            void* _shm_ptr;
    };
};

#endif