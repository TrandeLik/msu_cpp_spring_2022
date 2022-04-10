#include "token_parser.hpp"

void TokenParser::Parse(const std::string &s) {
    std::stringstream input(s);
    if (start_callback) {
        start_callback();
    }
    std::string token;
    while (input >> token) {
        try {
            if (!only_digits(token)) {
                throw std::exception();
            }
            uint64_t x = std::stoull(token);
            if (digit_callback) {
                digit_callback(x);
            }
        } catch (std::exception const&) {
            if (string_callback) {
                string_callback(token);
            }
        }
    }
    if (end_callback) {
        end_callback();
    }
}
