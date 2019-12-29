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

#include <exception>
#include <locale>
#include <codecvt>

#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#include "../../include/db.hpp"

alita::alita_db::alita_db(const alita::db_connection_info& db_connection_info)
{
    if(db_connection_info._host.empty())
        throw std::invalid_argument("Invalid Host.");

    if(db_connection_info._username.empty())
        throw std::invalid_argument("Invalid Username.");

    if(db_connection_info._password.empty())
        throw std::invalid_argument("Invalid Password.");

    if(db_connection_info._scheme.empty())
        throw std::invalid_argument("Invalid Scheme.");

    this->_log = db_connection_info._log;
    this->_driver = get_driver_instance();
    
    sql::ConnectOptionsMap map;
    map["hostName"] = db_connection_info._host;
    map["username"] = db_connection_info._username;
    map["password"] = db_connection_info._password;
    map["schema"] = db_connection_info._scheme;
    map["OPT_RECONNECT"] = true;
    map["CLIENT_MULTI_STATEMENTS"] = true;
    map["OPT_CHARSET_NAME"] = "utf8";
    
    this->_connection = this->_driver->connect(map);

    this->_connection->setSchema(db_connection_info._scheme);  
    this->_connection->setAutoCommit(true);
}

alita::alita_db::~alita_db()
{
    delete this->_connection;
}

int alita::alita_db::add_cache(std::wstring link, std::wstring content)
{
    if(link.empty())
        throw std::invalid_argument("Invalid link");

    if(content.empty())
        throw std::invalid_argument("Invalid content");

    int id = 0;
    
    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt;
        
        pstmt.reset(this->_connection->prepareStatement("CALL usp_AddCache(?, ?, @out)"));

        pstmt->setString(1, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(link));
        pstmt->setString(2, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(content));
        
        if(!pstmt->execute())
            throw std::runtime_error("Unable to execute...");
        
        this->clear(pstmt);
        
        pstmt.reset(this->_connection->prepareStatement("SELECT @out AS ID"));
        if(!pstmt->execute())
            throw std::runtime_error("Unable to execute...");
        
        std::unique_ptr<sql::ResultSet> res;
        res.reset(pstmt->getResultSet());
        while(res->next())
            id = res->getInt("ID");

        res->close();
        pstmt->close();
    }
    catch(const sql::SQLException& e)
    {
        std::cerr << "Error : " << e.what() << '\n';
        std::cerr << "Error Code : " << e.getErrorCode() << '\n';
        std::cerr << "SQL State : " << e.getSQLState() << '\n';
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    return id;
}

void alita::alita_db::add_index(std::wstring word, std::wstring link, int frequency)
{
    if(word.empty())
        throw std::invalid_argument("Invalid link");

    if(link.empty())
        throw std::invalid_argument("Invalid content");
    
    if(frequency <= 0)
        throw std::invalid_argument("Invalid frequency");

    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt;

        pstmt.reset(this->_connection->prepareStatement("CALL usp_AddIndex(?, ?, ?)"));

        pstmt->setString(1, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(word));
        pstmt->setString(2, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(link));
        pstmt->setInt(3, frequency);
        
        if(!pstmt->execute())
            throw std::runtime_error("Unable to execute...");
        
        this->clear(pstmt);
    }
    catch(const sql::SQLException& e)
    {
        std::cerr << "Error : " << e.what() << '\n';
        std::cerr << "Error Code : " << e.getErrorCode() << '\n';
        std::cerr << "SQL State : " << e.getSQLState() << '\n';
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
    }
}

alita::cache alita::alita_db::get_cache_by_id(int id)
{
    if(id <= 0)
        throw std::invalid_argument("Invalid id");

    alita::cache cache;

    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt;
        std::unique_ptr<sql::ResultSet> res;

        pstmt.reset(this->_connection->prepareStatement("CALL usp_GetCacheById(?)"));
        pstmt->setInt(1, id);
        
        if(!pstmt->execute())
            throw std::runtime_error("Unable to execute...");
        
        res.reset(pstmt->getResultSet());
        
        while (res->next()) {
            cache._id = res->getInt("Id");
            cache._link = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(res->getString("Link"));
            cache._content = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(res->getString("Content"));
            cache._created = res->getString("Created");
            cache._modified = res->getString("Modified");
            cache._process_state = res->getInt("ProcessState");
        }; 

        while(pstmt->getMoreResults())
            res.reset(pstmt->getResultSet());
        
        pstmt->close();
    }
    catch(const sql::SQLException& e)
    {
        std::cerr << "Error : " << e.what() << '\n';
        std::cerr << "Error Code : " << e.getErrorCode() << '\n';
        std::cerr << "SQL State : " << e.getSQLState() << '\n';
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return cache;
}

std::vector<alita::search_result> alita::alita_db::search(std::wstring word, int last_id = 0)
{
    if(word.empty())
        throw std::invalid_argument("Invalid word");
    
    if(last_id < 0)
        throw std::invalid_argument("Invalid id");

    std::vector<alita::search_result> results;

    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt;
        std::unique_ptr<sql::ResultSet> res;

        pstmt.reset(this->_connection->prepareStatement("CALL usp_Search(?, ?)"));

        pstmt->setString(1, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(word));
        pstmt->setInt(2, last_id);
        
        if(!pstmt->execute())
            throw std::runtime_error("Unable to execute...");
        
        res.reset(pstmt->getResultSet());
        while(res->next()) {
            alita::search_result s;
            s._index_id = res->getInt("IndexId");
            s._frequency = res->getInt("Frequency");
            s._last_indexed = res->getString("LastIndexed");
            s._page = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(res->getString("Page"));
            s._word = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(res->getString("Word"));
            results.push_back(s);
        }
        
        while(pstmt->getMoreResults())
            res.reset(pstmt->getResultSet());
        
        pstmt->close();
    }
    catch(const sql::SQLException& e)
    {
        std::cerr << "Error : " << e.what() << '\n';
        std::cerr << "Error Code : " << e.getErrorCode() << '\n';
        std::cerr << "SQL State : " << e.getSQLState() << '\n';
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return results;
}

void alita::alita_db::set_cache_state(int link_id, short state)
{
    std::cerr << "Setting cache state" << std::endl;

    if(link_id <= 0)
        throw std::invalid_argument("Invalid link id");

    if(state <= 0)
        throw std::invalid_argument("Invalid state");

    try
    {
        std::unique_ptr<sql::PreparedStatement> pstmt;

        pstmt.reset(this->_connection->prepareStatement("CALL usp_SetCacheState(?, ?)"));

        pstmt->setInt(1, link_id);
        pstmt->setInt(2, state);
        
        if(!pstmt->executeUpdate())
            throw std::runtime_error("Unable to execute...");
        
        this->clear(pstmt);
    }
    catch(const sql::SQLException& e)
    {
        std::cerr << "Error : " << e.what() << '\n';
        std::cerr << "Error Code : " << e.getErrorCode() << '\n';
        std::cerr << "SQL State : " << e.getSQLState() << '\n';
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
    }

    std::cerr << "Set cache state" << std::endl;
}

void alita::alita_db::log(std::string message)
{
    if(this->_log)
        std::cerr << message << std::endl;
}


void alita::alita_db::clear(std::unique_ptr<sql::Statement>& statement)
{
    std::unique_ptr<sql::ResultSet> res;
    do
    {
        res.reset(statement->getResultSet());
        res->close();
    } while (statement->getMoreResults()); 

    statement->close();   
}


void alita::alita_db::clear(std::unique_ptr<sql::PreparedStatement>& statement)
{
    std::unique_ptr<sql::ResultSet> res;
    do
    {
        res.reset(statement->getResultSet());
        res->close();
    } while (statement->getMoreResults());    

    statement->close();
}