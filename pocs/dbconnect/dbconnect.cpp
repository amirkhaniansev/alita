/**
 * GNU General Public License Version 3.0, 29 June 2007
 * POC for MySql database connect.
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

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


int main(int argc, char** argv)
{
    try
    {
        int id;

        sql::Driver* driver = get_driver_instance();
        
        std::cerr << "Connecting..." << std::endl;
        sql::Connection* conn = driver->connect("tcp://127.0.0.1:3306", "sev", "password");
        
        std::cerr << "Setting scheme..." << std::endl;
        conn->setSchema("Alita");

        std::unique_ptr<sql::Statement> stmt;
        std::unique_ptr<sql::PreparedStatement> pstmt;
        std::unique_ptr<sql::ResultSet> res;

        stmt.reset(conn->createStatement());
        pstmt.reset(conn->prepareStatement("CALL usp_AddCache(?, ?, @_linkId)"));

        pstmt->setString(1, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(L"AAA"));
        pstmt->setString(2, std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().to_bytes(L"AAA"));
        
        pstmt->execute();
        pstmt->clearParameters();
        res.reset(pstmt -> getResultSet());
        while(res->next());
        
        res.reset(pstmt->executeQuery("SELECT @_linkId AS Id"));
        while (res->next()) {
            id = std::stoi(res->getString("Id"));   
        }    
    }
    catch(const sql::SQLException& e)
    {
        std::cerr << "Error : " << e.what() << std::endl;
        std::cerr << "SQL Error Code : " << e.getErrorCode() << std::endl;
        std::cerr << "SQL Error State : " << e.getSQLState() << std::endl;
    }
    
    return 0;
}