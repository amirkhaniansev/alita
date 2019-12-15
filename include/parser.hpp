/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Header file for parser.
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

#ifndef __HTML_PARSER_HPP__
#define __HTML_PARSER_HPP__

#include <string>
#include <unordered_set>

#include <myhtml/api.h>

namespace alita {
    class html_parser {
        public:
            html_parser(std::string base_url, std::string content);
            void parse();
            std::string get_content();
            std::unordered_set<std::string> get_links();
            ~html_parser();
        private:
            std::string base_url;
            std::string content;
            myhtml* html;
            myhtml_tree* tree;
            myhtml_collection* collection;
    };
};

#endif