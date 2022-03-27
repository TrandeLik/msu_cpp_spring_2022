#include <gtest/gtest.h>
#include "test_allocator.h"

TEST_F(TestAllocator, invalid_size) {
    a->makeAllocator(30);
    ASSERT_EQ(nullptr, a->alloc(-1));
}

TEST_F(TestAllocator, no_memory) {
    a->makeAllocator(30);
    ASSERT_EQ(nullptr, a->alloc(40));
    a->alloc(10);
    ASSERT_EQ(nullptr, a->alloc(20));
}

TEST_F(TestAllocator, correct_allocation) {
    a->makeAllocator(30);
    auto first_ptr = a->alloc(10);
    ASSERT_NE(first_ptr, nullptr);
    auto second_ptr = a->alloc(10);
    ASSERT_NE(second_ptr, nullptr);
    ASSERT_EQ(10, second_ptr - first_ptr);
}

TEST_F(TestAllocator, reset) {
    a->makeAllocator(30);
    auto first_ptr = a->alloc(10);
    a->reset();
    ASSERT_TRUE(a->alloc(10) == first_ptr);
}
