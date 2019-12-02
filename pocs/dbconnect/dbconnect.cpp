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

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>


int main(int argc, char** argv)
{
    try
    {
        sql::Driver* driver = get_driver_instance();
        
        std::cerr << "Connecting..." << std::endl;
        sql::Connection* conn = driver->connect("tcp://127.0.0.1:3306", "sev", "password");
        
        std::cerr << "Setting scheme..." << std::endl;
        conn->setSchema("SearchEngine");

        std::cerr << "Executing insert statetment..." << std::endl;
        sql::Statement* statement = conn->createStatement();
        statement->execute("INSERT INTO Cache VALUES (0, NOW(), NOW(), 'https://github.com', 'Content')");
        statement->execute("INSERT INTO Cache VALUES (0, NOW(), NOW(), 'https://github.com', 'Content')");
        delete statement;

        std::cerr << "Querying result..." << std::endl;
        std::cerr << "Preparing the statement..." << std::endl;
        sql::PreparedStatement* prepared = conn->prepareStatement("SELECT * FROM Cache");
        
        std::cerr << "Executing query..." << std::endl;
        sql::ResultSet* set = prepared->executeQuery();
        sql::ResultSetMetaData* metadata = set->getMetaData();

        std::cerr << "Reading result set..." << std::endl;
        std::cerr << "Row Count : " << set->rowsCount() << std::endl;
        std::cerr << "Column Count : " << metadata->getColumnCount() << std::endl;
        while(set->next()) {
            for(int i = 1; i <= metadata->getColumnCount(); i++)
                std::cerr << metadata->getColumnName(i) << " : " << set->getString(i) << std::endl;
        }
        
        delete set;
        delete prepared;
        delete conn;
    }
    catch(const sql::SQLException& e)
    {
        std::cerr << "Error : " << e.what() << std::endl;
        std::cerr << "SQL Error Code : " << e.getErrorCode() << std::endl;
        std::cerr << "SQL Error State : " << e.getSQLState() << std::endl;
    }
    
    return 0;
}