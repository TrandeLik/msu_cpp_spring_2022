#include <iostream>
#include <functional>
#include "token_parser.hpp"

int main() {
    TokenParser parser;
    std::string line;
    parser.SetStartCallback([]() {
        std::cout << "Start\n";
        return 0;
    });
    parser.SetEndCallback([]() {
        std::cout << "End\n";
        return 0;
    });
    parser.SetStringTokenCallback([](std::string a) {
        std::cout << "String " << a << '\n';
        return 0;
    });
    parser.SetDigitTokenCallback([](uint64_t a) {
        std::cout << "Digit " << a << '\n';
        return 0;
    });
    while (std::getline(std::cin, line)) {
        parser.Parse(line);
    }
}
