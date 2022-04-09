#pragma once
#include <functional>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>

class TokenParser {
 public:
    using void_f = std::function<int(void)>;
    using str_f = std::function<int(const std::string&)>;
    using digit_f = std::function<int(uint64_t)>;

    static bool only_digits(const std::string& x) {
        for (auto a: x) {
            if (a - '0' < 0 || a - '0' > 9) {
                return false;
            }
        }
        return true;
    }

 private:
    void_f start_callback;
    str_f string_callback;
    digit_f digit_callback;
    void_f end_callback;

 public:
    int start_counter = 0;
    int end_counter = 0;
    int digit_counter = 0;
    int string_counter = 0;

    void SetStartCallback(const void_f& f) {
        start_callback = f;
    }

    void SetEndCallback(const void_f& f) {
        end_callback = f;
    }

    void SetDigitTokenCallback(const digit_f& f) {
        digit_callback = f;
    }

    void SetStringTokenCallback(const str_f& f) {
        string_callback = f;
    }

    void Parse(const std::string& s);
};
