#pragma once
#include <iostream>
#include "error.hpp"

class Serializer {
  private:
    std::ostream& out_;
    template<typename Head, typename... Tail>
    Error process(Head h, Tail... tail);
    Error process(bool arg);
    Error process(uint64_t arg);
    template<typename T>
    Error process(T t);
  public:
    explicit Serializer(std::ostream& out): out_(out) {}

    template <typename T>
    Error save(T& object) {
        return object.serialize(*this);
    }

    template <typename... ArgsT>
    Error operator()(ArgsT... args) {
        return process(args...);
    }
};


template<typename Head, typename... Tail>
Error Serializer::process(Head h, Tail... tail) {
    auto err = process(h);
    if (err == Error::NoError) {
        out_ << ' ';
        return process(tail...);
    }
    return err;
}

template<typename T>
Error Serializer::process(T) {
    return Error::CorruptedArchive;
}