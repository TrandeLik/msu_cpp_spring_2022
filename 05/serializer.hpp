#pragma once
#include <iostream>
#include "error.hpp"

class Serializer {
 private:
    std::ostream& out_;
    template<typename Head, typename... Tail>
    Error process(const Head& h, const Tail&... tail);
    Error process(const bool& arg);
    Error process(const uint64_t& arg);
    template<typename T>
    Error process(const T& t);

 public:
    explicit Serializer(std::ostream& out): out_(out) {}

    template <typename T>
    Error save(T& object) {
        return object.serialize(*this);
    }

    template <typename... ArgsT>
    Error operator()(const ArgsT&... args) {
        return process(args...);
    }
};


template<typename Head, typename... Tail>
Error Serializer::process(const Head& h, const Tail&... tail) {
    auto err = process(h);
    if (err == Error::NoError) {
        out_ << ' ';
        return process(tail...);
    }
    return err;
}

template<typename T>
Error Serializer::process(const T&) {
    return Error::CorruptedArchive;
}
