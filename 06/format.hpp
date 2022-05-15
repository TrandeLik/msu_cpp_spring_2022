#pragma once
#include <sstream>
#include <string>
#include <set>

class format_error: public std::logic_error {
 public:
    explicit format_error(const std::string& s) : std::logic_error(s) {}
};

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

    void write() {}

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
    bool was_number = false;
    size_t number = 0;
    std::set<size_t> numbers;
    for (auto c : f) {
        if (is_number) {
            if (c >= '0' && c <= '9') {
                was_number = true;
                number *= 10;
                number += c - '0';
            } else if (c == '}') {
                if (!was_number) {
                    throw format_error("Empty brackets");
                }
                numbers.insert(number);
                number = 0;
                was_number = false;
                is_number = false;
            } else {
                throw format_error("Not a number after open bracket");
            }
        } else {
            if (c == '}') {
                throw format_error("Single }");
            } else if (c == '{') {
                is_number = true;
            }
        }
    }
    if (is_number) {
        throw format_error("Not finished brackets");
    }
    for (auto& n : numbers) {
        if (n < 0 || n >= size) {
            throw format_error("Incorrect arg position");
        }
    }
    return numbers.size() == size;
}

Format::Format(const std::string& f, size_t size) {
    current_iter = 0;
    if (check_format(f, size)) {
        s = f;
    } else {
        throw format_error("Incorrect elements count");
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
