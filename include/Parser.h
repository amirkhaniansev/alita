/**
* GNU General Public License Version 3.0, 29 June 2007
*
* This file is part of the "search engine **".
* Copyright (C) <2019>
*      Authors: <Vladimir Simonyan>   <simonyan.vlad@gmail.com>
*               <amirkhaniansev>      <amirkhanyan.sevak@gmail.com>
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
* along with this program.  If not, see http://www.gnu.org/licenses/.
**/

#include <string>
#include <iterator>
#include <set>
#include <regex>

class Parser
{
 public:
	std::set <std::string> GetLinks(const std::string text)
	{
		static const std::regex hl_regex("<a href=\"(.*?)\"", std::regex_constants::icase);

		return 
		{ 
			std::sregex_token_iterator(text.begin(), text.end(), hl_regex, 1),
		    std::sregex_token_iterator{} 
		};
	}
};