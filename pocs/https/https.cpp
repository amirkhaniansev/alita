/**
 * GNU General Public License Version 3.0, 29 June 2007
 * POC for HTTPS request.
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

#include <iostream>
#include <string>

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "../../include/httplib.hpp"

int main(int argc, char** argv)
{
    int port = 443;    
    std::string base_address = "github.com";
    httplib::SSLClient client(base_address.c_str(), port);
    
    std::cerr << "Sending request to github.com..." << std::endl;
    
    auto response = client.Get("/");
    if(response->status != 200) {
        std::cerr << "Error Code : " << response->status << std::endl;
        return 1;
    }

    std::cerr << "Printing body..." << std::endl;
    std::cerr << response->body << std::endl;
    
    return 0;
}