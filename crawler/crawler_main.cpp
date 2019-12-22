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

#include "../include/crawler.hpp"

int main(int argc, char** argv)
{
    std::string url = "";
    std::vector<std::string> links = {
        "https://www.list.am",
        "https://www.eli.am",
        "https://www.akumb.am",
        "https://www.news.am",
        "https://www.ysu.am",
        "https://www.blognews.am",
        "https://hy.wikipedia.org"
    };

    alita::crawler crawler(url);
    
    crawler.set_concurreny_level(4);
    crawler.set_cache_size(1000000);
    crawler.set_initial_list(links);
    crawler.set_log_flag(false);
    crawler.start();

    return 0;
}