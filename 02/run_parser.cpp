#include <iostream>
#include <functional>
#include "token_parser.hpp"

int main() {
    TokenParser parser;
    std::string line;
    parser.SetStartCallback([]() {
        std::cout << "Start\n";
    });
    parser.SetEndCallback([]() {
        std::cout << "End\n";
    });
    parser.SetStringTokenCallback([](std::string a) {
        std::cout << "String " << a << '\n';
    });
    while (std::getline(std::cin, line)) {
        parser.Parse(line);
    }
}


