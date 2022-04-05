#pragma once
#include <functional>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>

class TokenParser {
 private:
    std::function<int(void)> start_callback;
    std::function<int(const std::string &)> string_callback;
    std::function<int(uint64_t)> digit_callback;
    std::function<int(void)> end_callback;

 public:
    int start_counter = 0;
    int end_counter = 0;
    int digit_counter = 0;
    int string_counter = 0;

    void SetStartCallback(const std::function<int(void)>& f) {
        start_callback = f;
    }

    void SetEndCallback(const std::function<int(void)>& f) {
        end_callback = f;
    }

    void SetDigitTokenCallback(const std::function<int(uint64_t)>& f) {
        digit_callback = f;
    }

    void SetStringTokenCallback(const std::function<int(const std::string &)>& f) {
        string_callback = f;
    }

    void Parse(const std::string& s) {
        std::stringstream input(s);
        if (start_callback) {
            ++start_counter;
            start_callback();
        }
        std::string token;
        while (input >> token) {
            try {
                uint64_t x = std::stoull(token);
                if (digit_callback) {
                    ++digit_counter;
                    digit_callback(x);
                }
            } catch (std::exception const&) {
                if (string_callback) {
                    ++string_counter;
                    string_callback(token);
                }
            }
        }
        if (end_callback) {
            ++end_counter;
            end_callback();
        }
    }
};
