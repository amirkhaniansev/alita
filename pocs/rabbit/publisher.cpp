/**
 * GNU General Public License Version 3.0, 29 June 2007
 * POC for RabbitMQ publisher.
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
#include <thread>
#include <chrono>

#include <SimpleAmqpClient/SimpleAmqpClient.h>

int main(int argc, char** argv)
{
    auto topic = "TestTopic";
    auto routing = "";

    try
    {
        std::cerr << "Creating channel..." << std::endl;
        auto channel = AmqpClient::Channel::Create();

        std::cerr << "Declaring topic..." << std::endl;
        channel->DeclareExchange(topic, AmqpClient::Channel::EXCHANGE_TYPE_TOPIC);

        while (true)
        {
            std::cerr << "Sleeping for 2 seconds..." << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            
            std::cerr << "Constructing messages..." << std::endl;
            auto message0 = AmqpClient::BasicMessage::Create("Ola!!!");
            auto message1 = AmqpClient::BasicMessage::Create("Hi!!!");

            std::cerr << "Publishing messages..." << std::endl;
            channel->BasicPublish(topic, routing, message0);
            channel->BasicPublish(topic, routing, message1);
        }
    }
    catch(std::exception& e)
    {
        std::cerr << "Error : " << e.what() << std::endl;
    }

    return 0;
}