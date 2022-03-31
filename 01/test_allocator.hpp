#pragma once

#include <gtest/gtest.h>
#include "allocator.hpp"

class TestAllocator : public ::testing::Test {
 protected:
    Allocator* a;
    void SetUp() {
        a = new Allocator;
    }
    void TearDown() {
        delete a;
    }
};
