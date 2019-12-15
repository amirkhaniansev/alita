/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Source file for publisher.
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

#include "../include/publisher.hpp"

alita::cpub::cpub(std::string uri)
{
    this->channel = uri.empty()
        ? AmqpClient::Channel::Create()
        : AmqpClient::Channel::CreateFromUri(uri);
}

void alita::cpub::declare(std::string topic)
{
    if(topic.empty())
        throw std::invalid_argument("INVALID TOPIC...");

    if(this->topics.find(topic) != this->topics.end())
        return;

    this->topics.insert(topic);    
    this->channel->DeclareExchange(topic, AmqpClient::Channel::EXCHANGE_TYPE_TOPIC);
}

void alita::cpub::publish(std::string topic, std::string message)
{
    if(topic.empty())
        throw std::invalid_argument("INVALID TOPIC...");

    if(message.empty())
        throw std::invalid_argument("INVALID MESSAGE...");

    if(this->topics.find(topic) == this->topics.end())
        throw std::invalid_argument("TOPIC IS NOT DECLARED...");
    
    this->channel->BasicPublish(topic, std::string(), AmqpClient::BasicMessage::Create(message));
}