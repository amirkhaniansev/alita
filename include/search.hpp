/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Header file for search library.
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

#ifndef __SEARCH_HPP__
#define __SEARCH_HPP__

#include <string>
#include <vector>
#include <unordered_map>

#include "db.hpp"

namespace alita {
    struct query {
        std::wstring _query;
        int _last_id;
    };

    class search {
        public:
            search(const alita::db_connection_info& info);
            std::unordered_map<std::wstring, alita::search_result> query(const alita::query& query);
        private:
            alita::alita_db _db;
    };
};

#endif