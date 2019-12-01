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