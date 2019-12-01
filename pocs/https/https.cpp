#include <iostream>
#include <string>

#define CPPHTTPLIB_OPENSSL_SUPPORT

#include "../../include/httplib.hpp"

int main(int argc, char** argv)
{
    int port = 443;    
    std::string base_address = "github.com";
    httplib::SSLClient client(base_address.c_str(), port);
    
    std::cerr << "Sending request to github.com..." << std::endl;
    
    auto response = client.Get("/");
    if(response->status != 200) {
        std::cerr << "Error Code : " << response->status << std::endl;
        return 1;
    }

    std::cerr << "Printing body..." << std::endl;
    std::cerr << response->body << std::endl;
    
    return 0;
}