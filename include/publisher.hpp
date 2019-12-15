/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Header file for publisher.
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

#ifndef __PUBLISHER_HPP__
#define __PUBLISHER_HPP__

#include <string>
#include <unordered_set>

#include <boost/shared_ptr.hpp>
#include <SimpleAmqpClient/SimpleAmqpClient.h>

namespace alita {
    class cpub {
        public:
            cpub(std::string uri);
            void declare(std::string topic);
            void publish(std::string topic, std::string message);
        private:
            AmqpClient::Channel::ptr_t channel;
            std::unordered_set<std::string> topics;
    };
};

#endif