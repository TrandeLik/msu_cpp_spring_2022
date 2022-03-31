#include <gtest/gtest.h>
#include "test_allocator.hpp"

TEST_F(TestAllocator, invalid_size) {
    a->makeAllocator(30);
    ASSERT_EQ(nullptr, a->alloc(-1));
}

TEST_F(TestAllocator, no_memory) {
    a->makeAllocator(30);
    ASSERT_EQ(nullptr, a->alloc(40));
    a->alloc(10);
    ASSERT_EQ(nullptr, a->alloc(30));
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

TEST_F(TestAllocator, alloc_before_make) {
    auto p = a->alloc(10);
    ASSERT_EQ(nullptr, p);
    a->makeAllocator(10);
    p = a->alloc(10);
    ASSERT_NE(nullptr, p);
}

TEST_F(TestAllocator, multiple_make) {
    a->makeAllocator(10);
    auto p = a->alloc(100);
    ASSERT_EQ(nullptr, p);
    p = a->alloc(10);
    ASSERT_NE(nullptr, p);
    a->makeAllocator(100);
    p = a->alloc(100);
    ASSERT_NE(nullptr, p);
    p = a->alloc(1000);
    ASSERT_EQ(nullptr, p);
    a->makeAllocator(1000);
    p = a->alloc(1000);
    ASSERT_NE(nullptr, p);
}

TEST_F(TestAllocator, short_chain) {
    a->makeAllocator(1);
    auto p = a->alloc(1);
    ASSERT_NE(nullptr, p);
    p = a->alloc(1);
    ASSERT_EQ(nullptr, p);
}

TEST_F(TestAllocator, long_chain) {
    a->makeAllocator(50);
    auto p1 = a->alloc(20);
    ASSERT_NE(nullptr, p1);
    auto p2 = a->alloc(30);
    ASSERT_NE(nullptr, p2);
    ASSERT_EQ(20, p2 - p1);
    auto p3 = a->alloc(1);
    ASSERT_EQ(nullptr, p3);
    a->reset();
    p2 = a->alloc(50);
    ASSERT_NE(nullptr, p2);
    ASSERT_EQ(p1, p2);
}

TEST_F(TestAllocator, ptr_diff) {
    a->makeAllocator(30);
    auto p1 = a->alloc(10);
    ASSERT_NE(nullptr, p1);
    auto p2 = a->alloc(10);
    ASSERT_NE(nullptr, p2);
    auto p3 = a->alloc(10);
    ASSERT_NE(nullptr, p3);
    ASSERT_EQ(10, p2 - p1);
    ASSERT_EQ(10, p3 - p2);
    ASSERT_EQ(20, p3 - p1);
}
