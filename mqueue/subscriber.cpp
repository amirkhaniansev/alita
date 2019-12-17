/**
 * GNU General Public License Version 3.0, 29 June 2007
 * Source file for subsriber.
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

#include "../include/subscriber.hpp"

alita::csub::csub(std::string uri)
{
    this->channel = uri.empty()
        ? AmqpClient::Channel::Create()
        : AmqpClient::Channel::CreateFromUri(uri);
}

void alita::csub::subsribe(std::string topic)
{
    if(topic.empty())
        throw std::invalid_argument("INVALID TOPIC...");

    if(this->tags.find(topic) != this->tags.end())
        return;

    auto queue = this->channel->DeclareQueue(std::string());
    this->channel->BindQueue(queue, topic, std::string());
    this->queues.insert(std::make_pair(topic, queue));

    auto tag = this->channel->BasicConsume(queue);
    this->tags.insert(std::make_pair(topic, tag));
}

std::string alita::csub::get_message(std::string topic)
{
    if(topic.empty())
        throw std::invalid_argument("INVALID TOPIC...");

    if(this->tags.find(topic) == this->tags.end())
        throw std::invalid_argument("TOPIC IS NOT SUBSCRIBED...");

    AmqpClient::Envelope::ptr_t env;
    if(!this->channel->BasicConsumeMessage(this->tags[topic], env))
        throw std::runtime_error("UNABLE TO GET MESSAGE...");

    return env->Message()->Body();
}