/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Header file for crawler.
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

#ifndef __CRAWLER_HPP__
#define __CRAWLER_HPP

#include <string>
#include <vector>

#include "publisher.hpp"

namespace alita {
    class crawler {
        public:
            crawler(std::string publish_url);
            void set_concurreny_level(const std::size_t& concurrency_level);
            void set_cache_size(const std::size_t& cache_size);
            void set_initial_list(const std::vector<std::string>& links);
            void set_log_flag(const bool& flag);
            void start();
        protected:
            void log(std::string title, std::string  content);
            void add(std::string link);
        private:
            std::string _url;
            std::size_t _concurrency_level;
            std::size_t _cache_size;
            std::vector<std::string> _links;
            std::unordered_set<std::string> _cache;
            bool _log;
            cpub _publisher;
    };
};

#endif