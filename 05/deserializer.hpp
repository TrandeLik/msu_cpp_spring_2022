#pragma once
#include <iostream>
#include "error.hpp"

class Deserializer {
private:
    std::istream& in_;
    template<typename Head, typename... Tail>
    Error process(Head& h, Tail&... tail);
    Error process(bool& arg);
    Error process(uint64_t& arg);
    template<typename T>
    Error process(T t);
public:
    explicit Deserializer(std::istream& in): in_(in) {}

    template <typename T>
    Error load(T& object) {
        return object.serialize(*this);
    }

    template <typename... ArgsT>
    Error operator()(ArgsT&... args) {
        return process(args...);
    }
};


template<typename Head, typename... Tail>
Error Deserializer::process(Head& h, Tail&... tail) {
    auto err = process(h);
    if (err == Error::NoError) {
        return process(tail...);
    }
    return err;
}

template<typename T>
Error Deserializer::process(T) {
    return Error::CorruptedArchive;
}
