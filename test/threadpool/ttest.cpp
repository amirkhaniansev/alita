/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Test for thread pool library.
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

#include "../../include/threadpool.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

void* print(void* message)
{
    void* result;
    static int mess_no;
    std::string msg = "WORK : ";
    msg += std::to_string(mess_no++);
    msg += " : ";
    msg += (char*)message;
    msg += '\n';

    std::cout << msg;

    for(auto i = 0; i < UINT16_MAX * 1000; i++);

    return result;
}

void* call(void* in, void* out)
{
    std::cout << "CALLBACK CALLED \n";
    return NULL;
}

int main(int argc, char** argv)
{
    alita::input_t input = (alita::input_t)"message";

    alita::threadpool::instance().enqueue_work(print, input, call);
    alita::threadpool::instance().enqueue_work(print, input, call);
    alita::threadpool::instance().enqueue_work(print, input, call);
    alita::threadpool::instance().enqueue_work(print, input, call);
    alita::threadpool::instance().state();
    alita::threadpool::instance().exit();
}