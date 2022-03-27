#include <gtest/gtest.h>
#include "allocator.h"

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
