#pragma once

#include <gtest/gtest.h>
#include <string>
#include "token_parser.hpp"

int start() {
    return 0;
}

int end() {
    return 0;
}

int str(const std::string&) {
    return 0;
}

int digit(uint64_t) {
    return 0;
}

class TestParser: public testing::Test {
 protected:
    TokenParser* parser;

    void SetUp() {
        parser = new TokenParser;
    }

    void TearDown() {
        delete parser;
    }
};
