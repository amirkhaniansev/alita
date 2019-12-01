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