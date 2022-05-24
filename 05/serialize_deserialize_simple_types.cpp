#include "error.hpp"
#include "deserializer.hpp"
#include "serializer.hpp"

Error Deserializer::process(bool& arg) {
    std::string data;
    in_ >> data;
    if (data == "false") {
        arg = false;
    } else if (data == "true") {
        arg = true;
    } else {
        return Error::CorruptedArchive;
    }
    return Error::NoError;
}

Error Deserializer::process(uint64_t& arg) {
    std::string data;
    in_ >> data;
    try {
        arg = std::stoull(data);
    } catch (std::exception&) {
        return Error::CorruptedArchive;
    }
    return Error::NoError;
}

Error Serializer::process(const bool& arg) {
    if (arg) {
        out_ << "true";
    } else {
        out_ << "false";
    }
    return Error::NoError;
}

Error Serializer::process(const uint64_t& arg) {
    out_ << arg;
    return Error::NoError;
}
