/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Test for links queue dequeing.
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

#include <unistd.h>
#include <stdlib.h>

#include <iostream>

#include "../../include/lqueue.hpp"

int main(int argc, char** argv)
{
    alita::lqueue::set_size(4000);
    alita::lqueue::instance().dump();

    try
    {
        int n = 0;
        int count = 0;
        alita::queue_item item;
        
        while (count++ < 1500)
        {
            sleep(1);
            n = rand() % 3;
            
            std::cerr << "DUMPING BEFORE..." << std::endl;
            alita::lqueue::instance().dump();

            for(int i = 0; i < n; i++) {
                alita::lqueue::instance().dequeue(&item);
                std::cerr << "DEQUEUED LINK : " << item._link._link << std::endl; 
            }

            std::cerr << "DUMPING AFTER..." << std::endl;
            alita::lqueue::instance().dump();
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << "ERROR : " << e.what() << std::endl;
        try
        {
            alita::lqueue::instance().unsync();
        }
        catch(const std::exception& e)
        {
            std::cerr << "UNSYNC ERROR : " << std::endl;
            std::cerr << e.what() << std::endl;
        }
        
    }
    
    alita::lqueue::instance().detach();
    alita::lqueue::instance().destroy();
    
    return 0;
}