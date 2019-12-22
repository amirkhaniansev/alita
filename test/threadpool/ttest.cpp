#include "../../include/threadpool.hpp"

#include <iostream>
#include <string>
#include <thread>
#include <chrono>

void* print(void* message)
{
    void* result;
    static int mess_no;
    std::string msg = "WORK : ";
    msg += std::to_string(mess_no++);
    msg += " : ";
    msg += (char*)message;
    msg += '\n';

    std::cout << msg;

    for(auto i = 0; i < UINT16_MAX * 1000; i++);

    return result;
}

void* call(void* in, void* out)
{
    std::cout << "CALLBACK CALLED \n";
    return NULL;
}

int main(int argc, char** argv)
{
    alita::input_t input = (alita::input_t)"message";

    alita::threadpool::instance().enqueue_work(print, input, call);
    alita::threadpool::instance().enqueue_work(print, input, call);
    alita::threadpool::instance().enqueue_work(print, input, call);
    alita::threadpool::instance().enqueue_work(print, input, call);
    alita::threadpool::instance().state();
    alita::threadpool::instance().exit();
}