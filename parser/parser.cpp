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

#include <vector>
#include <locale>
#include <codecvt>
#include <iostream>

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
}

std::unordered_set<std::string> alita::html_parser::get_links()
{    
    auto coll = myhtml_get_nodes_by_attribute_key(this->tree, NULL, NULL, "href", 4, NULL);

    std::unordered_set<std::string> set;
    for(auto i = 0; i < coll->length; i++) {
        auto node = coll->list[i];
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

    myhtml_collection_destroy(coll);
    
    return set;
}

std::unordered_map<std::wstring, int> alita::html_parser::get_content()
{
    std::unordered_map<std::wstring, int> result;

    std::vector<int> tags = {
        MyHTML_TAG_TT,
        MyHTML_TAG_I,
        MyHTML_TAG_B,
        MyHTML_TAG_BIG,
        MyHTML_TAG_SMALL,
        MyHTML_TAG_EM,
        MyHTML_TAG_STRONG,
        MyHTML_TAG_DFN,
        MyHTML_TAG_CODE,
        MyHTML_TAG_SAMP,
        MyHTML_TAG_KBD,
        MyHTML_TAG_VAR,
        MyHTML_TAG_CITE,
        MyHTML_TAG_ABBR,
        MyHTML_TAG_ACRONYM,
        MyHTML_TAG_SUB,
        MyHTML_TAG_SUP,
        MyHTML_TAG_SPAN,
        MyHTML_TAG_BDO,
        MyHTML_TAG_ADDRESS,
        MyHTML_TAG_DIV,
        MyHTML_TAG_A,
        MyHTML_TAG_OBJECT,
        MyHTML_TAG_P,
        MyHTML_TAG_H1,
        MyHTML_TAG_H2,
        MyHTML_TAG_H3,
        MyHTML_TAG_H4,
        MyHTML_TAG_H5,
        MyHTML_TAG_H6,
        MyHTML_TAG_PRE,
        MyHTML_TAG_Q,
        MyHTML_TAG_INS,
        MyHTML_TAG_DEL,
        MyHTML_TAG_DT,
        MyHTML_TAG_DD,
        MyHTML_TAG_LI,
        MyHTML_TAG_LABEL,
        MyHTML_TAG_OPTION,
        MyHTML_TAG_TEXTAREA,
        MyHTML_TAG_FIELDSET,
        MyHTML_TAG_LEGEND,
        MyHTML_TAG_CAPTION,
        MyHTML_TAG_TD,
        MyHTML_TAG_TH,
        MyHTML_TAG_TITLE,
        MyHTML_TAG_SCRIPT,
        MyHTML_TAG_STYLE
    };

    for(auto lit = tags.begin(); lit != tags.end(); lit++) {
        auto coll = myhtml_get_nodes_by_tag_id(this->tree, NULL, *lit, NULL);
        if(coll == NULL)
            continue;
        
        for(auto cit = 0; cit < coll->length; cit++) {
            auto node = myhtml_node_child(coll->list[cit]);
            if(node == NULL)
                continue;
            
            auto length = 0UL;
            auto str = myhtml_node_text(node, &length);
            if(str == NULL)
                continue;

            auto cxxstr = std::string(str, length);
            auto text = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>().from_bytes(cxxstr);
            this->add(&result, text);
        }

        myhtml_collection_destroy(coll);
    }

    return result;
}


void alita::html_parser::add(std::unordered_map<std::wstring, int>* words, std::wstring& text)
{
    auto word = std::wstring();

    for(auto it = text.begin(); it != text.end(); it++) {
        if(*it != ' ') {
            word += *it;
            continue;
        }

        if(word.empty())
            continue;

        if(words -> find(word) == words -> end())
            words -> insert(std::make_pair(word, 1));
        else 
            (*words)[word]++;

        word.clear(); 
    }
}

alita::html_parser::~html_parser()
{  
    myhtml_tree_destroy(this->tree);
    myhtml_destroy(this->html);
}