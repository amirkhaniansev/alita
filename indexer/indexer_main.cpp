/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Source file containing indexer entry point.
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

#include "../include/db.hpp"
#include "../include/indexer.hpp"

int main(int argc, char** argv)
{
    std::string subscribe_url = "";

    alita::db_connection_info info;
    info._host = "tcp://127.0.0.1:3306";
    info._scheme = "Alita";
    info._username = "sev";
    info._password = "password";
    info._log = true;

    alita::indexer indexer(subscribe_url, info);
    indexer.start();
    
    return 0;
}