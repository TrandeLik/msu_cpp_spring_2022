#pragma once
#include <sstream>
#include <string>
#include <set>

class Format {
 private:
    std::string s;
    size_t current_iter;
    bool check_format(const std::string& f, size_t size);
 public:
    Format(const std::string& f, size_t size);

    template<typename Head>
    void write(const Head& h);

    template<typename Head, typename... Tail>
    void write(const Head& h, const Tail&... tail);

    operator std::string () {
        return s;
    }
};

template <typename... Args>
std::string format(const std::string& s, const Args&... a) {
    Format f(s, sizeof...(a));
    f.write(a...);
    return static_cast<std::string>(f);
}


bool Format::check_format(const std::string& f, size_t size) {
    bool is_number = false;
    size_t number = 0;
    std::set<size_t> numbers;
    for (auto c : f) {
        if (is_number) {
            if (c >= '0' && c <= '9') {
                number *= 10;
                number += c - '0';
            } else if (c == '}') {
                numbers.insert(number);
                number = 0;
                is_number = false;
            } else {
                return false;
            }
        } else {
            if (c == '}') {
                return false;
            } else if (c == '{') {
                is_number = true;
            }
        }
    }
    if (is_number) {
        return false;
    }
    for (auto& n : numbers) {
        if (n < 0 || n >= size) {
            return false;
        }
    }
    return numbers.size() <= size;
}

Format::Format(const std::string& f, size_t size) {
    current_iter = 0;
    if (check_format(f, size)) {
        s = f;
    } else {
        throw std::runtime_error("Incorrect format string for given args");
    }
}

template<typename Head>
void Format::write(const Head& h) {
    std::stringstream num;
    num << '{' << current_iter << '}';
    size_t pos;
    while ((pos = s.find(num.str())) != std::string::npos) {
        std::stringstream str_version_of_arg;
        str_version_of_arg << h;
        s.replace(pos, num.str().length(), str_version_of_arg.str());
    }
    ++current_iter;
}

template<typename Head, typename... Tail>
void Format::write(const Head& h, const Tail&... tail) {
    write(h);
    write(tail...);
}
