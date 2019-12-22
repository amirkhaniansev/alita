/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Source file for crawler.
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

#include "../include/crawler.hpp"
#include "../include/threadpool.hpp"
#include "../include/lqueue.hpp"
#include "../include/httpsc.hpp"
#include "../include/parser.hpp"

alita::crawler::crawler(std::string publish_url) : _publisher(publish_url)
{
    this->_url = publish_url;
    this->_concurrency_level = 1;
    this->_publisher.declare("Link Content");

    alita::lqueue::set_size(1000000000);
}

void alita::crawler::set_concurreny_level(const std::size_t& concurrency_level)
{
    if(concurrency_level == 0 || concurrency_level > 32)
        throw std::invalid_argument("Invalid Concurrency Level");

    this->_concurrency_level = concurrency_level;
}

void alita::crawler::set_cache_size(const std::size_t& cache_size)
{
    if(cache_size == 0 || cache_size > 1000000)
        throw std::invalid_argument("Invalid Cache Size");

    this->_cache_size = cache_size;
}

void alita::crawler::set_initial_list(const std::vector<std::string>& links)
{
    if(links.empty())
        throw std::invalid_argument("List of links is empty");

    for(auto it = links.begin(); it != links.end(); it++)
        if(it->size() > 2000)
            throw std::invalid_argument("List of links contains invalid link");

    this->_links = links;
    for(auto it = this->_links.begin(); it != this->_links.end(); it++)
        alita::lqueue::instance().enqueue(*it);
}

void alita::crawler::set_log_flag(const bool& flag)
{
    this->_log = flag;
}

void alita::crawler::start()
{
    while (true)
    {
        try
        {
            alita::queue_item item;
            alita::lqueue::instance().dequeue(&item);
            alita::lqueue::instance().dump();

            std::string url(item._link._link);
            this->log("Link Dequeued", url);
            
            std::size_t protocol = url.find("://");
            std::string host = url.substr(protocol == std::string::npos ? protocol + 1 : protocol + 3);
            this->log("Host", host);

            std::string content = alita::httpsc::get(url);
        
            std::string message = "{\"Link\":\"" + url + "\",\"Content\":" + content + "\"}";
            this->_publisher.publish("Link Content", message);

            if(content.empty())
                continue;

            alita::html_parser parser(host, content);
            parser.parse();

            std::unordered_set<std::string> links = parser.get_links();
            for(auto it = links.begin(); it != links.end(); it++) {
                if(*it == url || this->_cache.find(*it) != this->_cache.end())
                    continue;

                alita::lqueue::instance().enqueue(*it);
                this->add(*it);
                this->log("Link Enqueued", *it);
            }
        }
        catch(const std::exception& e)
        {
            std::string error = "Error";
            std::string exception = e.what();
            std::string overall_error = error + exception;

            std::cerr << overall_error << std::endl;

            alita::lqueue::instance().detach();
        }            
    }
}

void alita::crawler::log(std::string title, std::string content)
{
    if(this->_log)
        std::cerr << title << " : " << content << std::endl;
}

void alita::crawler::add(std::string link)
{
    // TODO cache should be chronogical
    if(this->_cache.size() == this->_cache_size)
        this->_cache.clear();

    this->_cache.insert(link);
}