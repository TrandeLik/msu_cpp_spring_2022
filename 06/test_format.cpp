#include <gtest/gtest.h>
#include "format.hpp"


class TestFormat: public testing::Test {
 protected:
    void SetUp() {
    }

    void TearDown() {
    }
};

class C {};
std::ostream& operator<<(std::ostream& out, C) {
    out << "2";
    return out;
}

TEST_F(TestFormat, simple_test) {
    auto result = format("{1}+{1} = {0}", 2, "one");
    ASSERT_EQ("one+one = 2", result);
}

TEST_F(TestFormat, different_order) {
    auto result = format("{1}+{0} = {1}", 2, "one");
    ASSERT_EQ("one+2 = one", result);
}

TEST_F(TestFormat, less_places) {
    auto result = format("{1}+{0} = {1}", 2, "one", "three");
    ASSERT_EQ("one+2 = one", result);
}

TEST_F(TestFormat, custom_class) {
    C c;
    auto result = format("{1}+{0} = {1}", c, "one", "three");
    ASSERT_EQ("one+2 = one", result);
}

TEST_F(TestFormat, incorrect_elements_in_format) {
    ASSERT_THROW(format("{1}+{0}", 1), std::runtime_error);
}

TEST_F(TestFormat, incorrect_open_bracket) {
    ASSERT_THROW(format("{{1}+{0}", 1, 2), std::runtime_error);
    ASSERT_THROW(format("{1}++{++{0}", 1, 3), std::runtime_error);
    ASSERT_THROW(format("{1}+{0}{", 1, 4), std::runtime_error);
}

TEST_F(TestFormat, incorrect_close_bracket) {
    ASSERT_THROW(format("}{1}+{0}", 1, 2), std::runtime_error);
    ASSERT_THROW(format("{1}++}++{0}", 1, 2), std::runtime_error);
    ASSERT_THROW(format("{1}+{0}}", 1, 2), std::runtime_error);
}

TEST_F(TestFormat, string_in_brackets) {
    ASSERT_THROW(format("{lol}", 1), std::runtime_error);
}

TEST_F(TestFormat, incorrect_number) {
    ASSERT_THROW(format("{-1}", 1), std::runtime_error);
    ASSERT_THROW(format("{2}", 1), std::runtime_error);
    ASSERT_THROW(format("{4096}", 1), std::runtime_error);
}

TEST_F(TestFormat, unfinished_number) {
    ASSERT_THROW(format("{1000", 1), std::runtime_error);
}

TEST_F(TestFormat, skip_element) {
    auto result = format("{0} {2}", 0, 1, 2);
    ASSERT_EQ("0 2", result);
}
