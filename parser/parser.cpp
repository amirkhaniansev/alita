/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Source for parser.
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

#include "../include/parser.hpp"

alita::html_parser::html_parser(std::string base_url, std::string content)
{
    if(base_url.empty())
        throw std::invalid_argument("INVALID BASE URL");

    if(content.empty())
        throw std::invalid_argument("INVALID CONTENT...");

    this->base_url = base_url;
    this->content = content;

    this->html = myhtml_create();
    myhtml_init(this->html, MyHTML_OPTIONS_DEFAULT, 1, 0);

    this->tree = myhtml_tree_create();
    myhtml_tree_init(this->tree, this->html);
}

void alita::html_parser::parse()
{
    myhtml_parse(this->tree, MyENCODING_UTF_8, this->content.c_str(), this->content.length());

    this->collection = myhtml_get_nodes_by_attribute_key(this->tree, NULL, NULL, "href", 4, NULL);
}

std::unordered_set<std::string> alita::html_parser::get_links()
{
    std::unordered_set<std::string> set;
    for(auto i = 0; i < this->collection->length; i++) {
        auto node = this->collection->list[i];
        for(auto j = myhtml_node_attribute_first(node); j != myhtml_node_attribute_last(node); j = myhtml_attribute_next(j)) {
            if(strcmp(myhtml_attribute_key(j, NULL), "href") != 0)
                continue;
            
            auto clink_size = 0UL;
            auto clink = myhtml_attribute_value(j, &clink_size);
            auto link = std::string(clink, clink_size);

            if(link[0] == '/')
                link = this->base_url + link;
            
            set.insert(link);
        }
    }

    return set;
}

alita::html_parser::~html_parser()
{
    myhtml_collection_destroy(this->collection);
    myhtml_tree_destroy(this->tree);
    myhtml_destroy(this->html);
}