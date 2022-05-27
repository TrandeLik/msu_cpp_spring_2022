#include <gtest/gtest.h>
#include <algorithm>
#include "vector.hpp"

// Capacity and size are tested in all testing functions

class TestVector: public testing::Test {
 protected:
    void SetUp() {
    }

    void TearDown() {
    }
};

TEST_F(TestVector, data_access) {
    Vector<int> a(10);
    ASSERT_EQ(10, a.size());
    ASSERT_EQ(10, a.capacity());
    for (int i = 0; i < 10; ++i) {
        a[i] = i;
    }
    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(i, a[i]);
    }
    a[0] = -10;
    ASSERT_EQ(-10, a[0]);
    ASSERT_EQ(10, a.size());
    ASSERT_EQ(10, a.capacity());
}

TEST_F(TestVector, push_back) {
    Vector<int> a;
    for (int i = 0; i < 10; ++i) {
        a.push_back(i + 10);
    }
    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(i + 10, a[i]);
    }
    ASSERT_EQ(10, a.size());
    ASSERT_EQ(15, a.capacity());
}

TEST_F(TestVector, emplace_back) {
    Vector<int> a;
    for (int i = 0; i < 10; ++i) {
        a.emplace_back(i + 12);
    }
    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(i + 12, a[i]);
    }
    ASSERT_EQ(10, a.size());
    ASSERT_EQ(15, a.capacity());
}

TEST_F(TestVector, pop_back) {
    Vector<int> a;
    for (int i = 0; i < 10; ++i) {
        a.push_back(i + 12);
    }
    for (int i = 0; i < 10; ++i) {
        ASSERT_EQ(10 - i, a.size());
        ASSERT_GE(2 * (10 - i), a.capacity());
        a.pop_back();
    }
    ASSERT_EQ(0, a.size());
    ASSERT_EQ(0, a.capacity());
}

TEST_F(TestVector, empty_and_clear) {
    Vector<int> a;
    ASSERT_TRUE(a.empty());
    a.push_back(1);
    ASSERT_FALSE(a.empty());
    a.clear();
    ASSERT_TRUE(a.empty());
    a.resize(10);
    ASSERT_FALSE(a.empty());
    a.clear();
    ASSERT_TRUE(a.empty());
    a.reserve(10);
    ASSERT_TRUE(a.empty());
    Vector<int> b(10);
    ASSERT_FALSE(b.empty());
    b.clear();
    ASSERT_TRUE(b.empty());
    b.emplace_back(1);
    ASSERT_FALSE(b.empty());
    Vector<int> c(b);
    ASSERT_FALSE(c.empty());
}

TEST_F(TestVector, test_reserve) {
    Vector<int> a;
    a.reserve(10);
    ASSERT_EQ(10, a.capacity());
    ASSERT_EQ(0, a.size());
    a.reserve(100);
    ASSERT_EQ(100, a.capacity());
    ASSERT_EQ(0, a.size());
    a.reserve(10);
    ASSERT_EQ(100, a.capacity());
    ASSERT_EQ(0, a.size());
    a.reserve(0);
    ASSERT_EQ(100, a.capacity());
    ASSERT_EQ(0, a.size());
    a.clear();
    ASSERT_EQ(100, a.capacity());
    ASSERT_EQ(0, a.size());
    Vector<int> b;
    b.push_back(1);
    ASSERT_EQ(1, b.capacity());
    ASSERT_EQ(1, b.size());
    b.push_back(1);
    ASSERT_EQ(3, b.capacity());
    ASSERT_EQ(2, b.size());
}

TEST_F(TestVector, resize) {
    Vector<int> a;
    a.resize(10);
    ASSERT_EQ(10, a.size());
    a.resize(100);
    ASSERT_EQ(100, a.size());
    a.resize(80);
    ASSERT_EQ(80, a.size());
    a.resize(0);
    ASSERT_EQ(0, a.size());
    a.reserve(1000);
    ASSERT_EQ(0, a.size());
    a.resize(10);
    ASSERT_EQ(10, a.size());
}

TEST_F(TestVector, forward_iterator) {
    Vector<int> a;
    for (int i = 0; i < 10; ++i) {
        a.emplace_back(i + 12);
    }
    int counter = 0;
    for (auto i = a.begin(); i < a.end(); ++i) {
        ASSERT_EQ(counter + 12, *i);
        ++counter;
    }
    Vector<int> b(3);
    b[0] = 1;
    b[1] = 10;
    b[2] = -1;
    std::sort(b.begin(), b.end());
    ASSERT_EQ(-1, b[0]);
    ASSERT_EQ(1, b[1]);
    ASSERT_EQ(10, b[2]);
}

TEST_F(TestVector, forward_iterator_features) {
    Vector<int> a;
    for (int i = 0; i < 10; ++i) {
        a.emplace_back(i + 12);
    }
    auto i = a.begin();
    ASSERT_EQ(12, i[0]);
    i += 2;
    ASSERT_EQ(14, *i);
    i -= 2;
    ASSERT_EQ(12, *i);
    auto j = a.end();
    j--;
    ASSERT_EQ(21, *j);
    ++j;
    ASSERT_EQ(j - i, 10);
}

TEST_F(TestVector, reverse_iterator) {
    Vector<int> a;
    for (int i = 0; i < 10; ++i) {
        a.emplace_back(i + 12);
    }
    int counter = 0;
    for (auto i = a.rbegin(); i < a.rend(); ++i) {
        ASSERT_EQ(21 - counter, *i);
        ++counter;
    }
}

TEST_F(TestVector, invalid_data_access) {
    Vector<int> a(10);
    try {
        a[10];
        FAIL();
    } catch (const std::out_of_range& er) {
        ASSERT_STREQ("Index out of range", er.what());
    }
    try {
        a[-1];
        FAIL();
    } catch (const std::out_of_range& er) {
        ASSERT_STREQ("Index out of range", er.what());
    }
    try {
        a[1000];
        FAIL();
    } catch (const std::out_of_range& er) {
        ASSERT_STREQ("Index out of range", er.what());
    }
}

TEST_F(TestVector, invalid_iterator_subtraction) {
    Vector<int> a(10);
    try {
        a.begin() - a.end();
        FAIL();
    } catch (const std::invalid_argument& er) {
        ASSERT_STREQ("Can't subtract following iterator from previous", er.what());
    }
}
