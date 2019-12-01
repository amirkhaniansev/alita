/**
 * GNU General Public License Version 3.0, 29 June 2007
 * POC for RabbitMQ subsriber.
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
    auto queue = "";
    auto routing = "";

    try
    {
        std::cerr << "Creating channel..." << std::endl;
        auto channel = AmqpClient::Channel::Create();
        
        std::cerr << "Declaring queue..." << std::endl;
        auto channel_queue = channel->DeclareQueue(queue);
        
        std::cerr << "Binding queue..." << std::endl;
        channel->BindQueue(channel_queue, topic, routing);

        std::cerr << "Consuming queue..." << std::endl;
        auto tag = channel->BasicConsume(channel_queue);

        std::cerr << "Getting..." << std::endl;
        
        while (true)
        {
            AmqpClient::Envelope::ptr_t env;
            if(!channel->BasicConsumeMessage(tag, env)) {
                std::cerr << "Deliver error..." << std::endl;
                continue;
            }

            std::cerr << "Message : " <<env->Message()->Body() << std::endl;
        }        
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    
    return 0;
}