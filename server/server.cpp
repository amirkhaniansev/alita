/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Source file for REST Search API server.
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

#include <locale>
#include <codecvt>

#include "../include/server.hpp"
#include "../include/search.hpp"
#include "../include/db.hpp"

alita::server::server(std::string host, int port)
{
    if(host.empty())
        throw std::invalid_argument("Invalid host");

    if(port < 0)
        throw std::invalid_argument("Invalid port");
    
    this->_host = host;
    this->_port = port;
}

void alita::server::start()
{
    if(this->_started)
        throw std::runtime_error("Service is already started");
    this->_started = true;

    this->_srv.Get(
        "/search",
        [&](const httplib::Request &req, httplib::Response &res)
        {
            try
            {
                std::cerr << "Request with " << req.path << std::endl;
                if(!req.has_param("query")) {
                    std::cerr << "Query parameter is missing " << std::endl;
                    res.status = 400;
                    res.set_content("No 'query' parameter is provided.", "text/plain");
                    return;
                }

                alita::db_connection_info info;
                info._host = "tcp://localhost:3306";
                info._scheme = "Alita";
                info._username = "sev";
                info._password = "password";
                info._log = true;

                alita::search s(info);

                std::string param = req.get_param_value("query");
                if(param.empty()) {
                    std::cerr << "Query Parameter is empty";
                    res.status = 400;
                    res.set_content("Invalid Parameter", "text/plain");
                    return;
                }

                alita::query query;
                query._last_id = 0;
                query._query = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(param);
                if(req.has_param("last_id"))
                    query._last_id = std::stoi(req.get_param_value("last_id"));

                std::unordered_map<std::wstring, alita::search_result> result = s.query(query);
                if(result.size() == 0) {
                    std::wcerr << L"No Record Found For" << query._query << std::endl;
                    res.status = 404;
                    res.set_content("No Record Found.", "text/plain");
                    return;
                }

                std::wstring wcontent = L"{\n";
                for(auto it = result.begin(); it != result.end(); it++) {
                    wcontent += L"\t\"" + it->first + L"\" : {\n\t\t" +
                                L"\"IndexID\" : " + std::to_wstring(it->second._index_id) + L",\n\t\t" + 
                                L"\"Word\" : \"" + it->second._word + L"\", \n\t\t" +
                                L"\"Page\" : \"" + it->second._page + L"\", \n\t\t" +
                                L"\"Frequency\" : " + std::to_wstring(it->second._frequency) + L",\n\t\t" +
                                L"\"LastIndexed\" : \"" + 
                                std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(it->second._last_indexed) + 
                                L"\", \n\t},\n";   
                }

                wcontent.erase(wcontent.size() - 1);
                wcontent.append(L"\n}");

                std::string content = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(wcontent);
                res.status = 200;
                res.set_content(content, "application/json");
            }
            catch(const std::exception& e)
            {
                std::cerr << "Error " << e.what() << std::endl;
                res.set_content("Error.", "text/plain");
                res.status = 500;
            }            
        });

    if(!this->_srv.listen(this->_host.c_str(), this->_port))
        throw std::runtime_error("Unable to start");
}

void alita::server::stop()
{
    if(!this->_started)
        throw std::runtime_error("Service is not started yet");
    this->_started = false;
    this->_srv.stop();
}