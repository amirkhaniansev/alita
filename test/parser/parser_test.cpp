#include <string>
#include <iostream>
#include <fstream>

#include "../../include/parser.hpp"

int main(int argc, char** argv)
{
    std::ifstream stream("../parser/content.html");
    std::string html((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());

    alita::html_parser parser("github.com", html);
    parser.parse();

    auto set = parser.get_links();
    for(auto it = set.begin(); it != set.end(); it++) {
        std::cout << "LINK : " << *it << std::endl;
    }

    return 0;
}