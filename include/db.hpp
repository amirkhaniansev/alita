/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Header file for database client interface.
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

#ifndef __DB_HPP__
#define __DB_HPP__

#include <string>
#include <vector>
#include <memory>

#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

namespace alita {
    struct db_connection_info {
        std::string _host;
        std::string _username;
        std::string _password;
        std::string _scheme;
        bool _log;
    };

    struct db_model {
        int _id;
        std::string _created;
    };

    struct db_modifiable_model : public db_model {
        std::string _modified;
    };

    struct cache : public db_modifiable_model {
        int _id;
        short _process_state;
        std::wstring _link;
        std::wstring _content;
    };

    struct word : public db_model {
        std::wstring _content;
    };

    struct index : public db_modifiable_model {
        int _word_id;
        int _link_id;
        int _frequency;
    };

    struct search_result {
        int _index_id;
        int _frequency;
        std::wstring _word;
        std::wstring _page;
        std::string _last_indexed;
    };

    class alita_db {
        public:
            alita_db(const db_connection_info& db_connection_info);
            int add_cache(std::wstring link, std::wstring content);
            void add_index(std::wstring word, std::wstring link, int frequency);
            void set_cache_state(int link_id, short state);
            alita::cache get_cache_by_id(int id);
            std::vector<alita::search_result> search(std::wstring word, int last_id);
            ~alita_db();
        private:
            sql::Driver* _driver;
            sql::Connection* _connection;
            bool _log;
            void log(std::string message);
            void clear(std::unique_ptr<sql::Statement>& statement);
            void clear(std::unique_ptr<sql::PreparedStatement>& statement);
    };
};

#endif