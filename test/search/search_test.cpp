/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Test for Seach Programmatic API.
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
#include <locale>
#include <codecvt>

#include "../../include/search.hpp"

int main(int argc, char** argv)
{
    alita::db_connection_info info;
    info._host = "tcp://127.0.0.1:3306";
    info._scheme = "Alita";
    info._username = "sev";
    info._password = "password";
    info._log = true;

    alita::search s(info);
    
    std::wstring input;
    alita::query query;

    try
    {
        std::wcout << L"Search Query : ";
        std::wcin >> input;

        std::wcout << L"Executing Search : " << std::endl; 
        query._query = input;
        query._last_id = 0;
        auto result = s.query(query);
        std::wcout << L"Search Executed..." << std::endl;

        std::wcout << "Reading..." << std::endl;
        for(auto it = result.begin(); it != result.end(); it++) {
            std::wcout << "Index ID : " << it->second._index_id << '\n'
                       << "Frequency : " << it->second._frequency << '\n'
                       << "Page : " << it->second._page << '\n'
                       << "Word : " << it->second._word << '\n'
                       << "Last Indexed : " 
                       << std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(it->second._last_indexed)
                       << '\n';
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
    return 0;
}
