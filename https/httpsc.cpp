/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Implementation for HTTP(S) client.
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
#include <iostream>
#include <string>

#include <curl/curl.h>

#include "../include/httpsc.hpp"

size_t write_data(void *contents, size_t size, size_t nmemb, void *userp)
{
    std::size_t real_size = size * nmemb;
    std::string* buff = (std::string*)userp;
    std::string content((char*)contents, real_size);
    
    buff->append(content);

    return real_size;
}

std::string alita::httpsc::get(const std::string url)
{
    CURL *curl;
    CURLcode res;
    std::string buffer;

    curl = curl_easy_init();
    if(!curl)
        return std::string();

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &buffer);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);

    return buffer;
}