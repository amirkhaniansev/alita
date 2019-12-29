/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Source file for search programmatic API.
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

#include <unordered_map>
#include <unordered_set>

#include "../include/search.hpp"

alita::search::search(const alita::db_connection_info& info) : _db(info)
{
}

std::unordered_map<std::wstring, alita::search_result> alita::search::query(const alita::query& query)
{
    if(query._query.empty())
        throw std::invalid_argument("Invalid query string");

    if(query._last_id < 0)
        throw std::invalid_argument("Invalid last Index ID");

    std::wcout << L"Searching For : " << query._query << std::endl;

    std::wstring word;
    std::unordered_set<std::wstring> words;
    std::unordered_map<std::wstring, alita::search_result> results;
    for(auto it = query._query.begin(); it != query._query.end(); it++) {
        if(*it != L' ' && it + 1 != query._query.end()) {
            word += *it; 
            continue;
        };

        if(word.empty() || words.find(word) != words.end()) {
            word.clear();
            continue;
        }

        std::wcout << L"Word Found " << word << std::endl;
        words.insert(word);

        auto result = this->_db.search(word, query._last_id);
        std::wcout << L"Result Count For : " << result.size() << std::endl;
        for(auto rit = result.begin(); rit != result.end(); rit++)
            results[rit->_page] = *rit;
        
        word.clear();
    }

    return results;
}