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

TEST_F(TestFormat, custom_class) {
    C c;
    auto result = format("{1}+{0} = {1}", c, "one");
    ASSERT_EQ("one+2 = one", result);
}

TEST_F(TestFormat, empty_string) {
    auto result = format("");
    ASSERT_EQ("", result);
}

TEST_F(TestFormat, without_formatting) {
    auto result = format("I am just a string");
    ASSERT_EQ("I am just a string", result);
}

TEST_F(TestFormat, one_argument) {
    auto result = format("{0}", 1);
    ASSERT_EQ("1", result);
}

TEST_F(TestFormat, long_string) {
    auto result = format("{0}{0}{0}{0}{0}{0}{0}{0}{0}{0}{0}", 1);
    ASSERT_EQ("11111111111", result);
    result = format("{0}{1}{0}{1}{0}{1}{0}{1}{0}{1}{0}", 1, 0);
    ASSERT_EQ("10101010101", result);
    result = format("|||{0}+{1}++{0}+++{1}={0}=={1}==={0}-{1}--{0}---{1}|{0}|||", 1, 0);
    ASSERT_EQ("|||1+0++1+++0=1==0===1-0--1---0|1|||", result);
}

TEST_F(TestFormat, incorrect_elements_in_format) {
    try {
        format("{1}+{0}", 1);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Incorrect arg position", er.what());
    }
}

TEST_F(TestFormat, incorrect_open_bracket) {
    try {
        format("{{1}+{0}", 1, 2);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Not a number after open bracket", er.what());
    }
    try {
        format("{1}++{++{0}", 1, 3);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Not a number after open bracket", er.what());
    }
    try {
        format("{1}+{0}{", 1, 4);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Not finished brackets", er.what());
    }
}

TEST_F(TestFormat, incorrect_close_bracket) {
    try {
        format("}{1}+{0}", 1, 2);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Single }", er.what());
    }
    try {
        format("{1}++}++{0}", 1, 2);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Single }", er.what());
    }
    try {
        format("{1}+{0}}", 1, 2);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Single }", er.what());
    }
}

TEST_F(TestFormat, string_in_brackets) {
    try {
        format("{lol}", 1);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Not a number after open bracket", er.what());
    }
    try {
        format("{0lol}", 1);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Not a number after open bracket", er.what());
    }
    try {
        format("{1lol}", 1);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Not a number after open bracket", er.what());
    }
}

TEST_F(TestFormat, incorrect_number) {
    try {
        format("{-1}", 1);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Not a number after open bracket", er.what());
    }
    try {
        format("{2}", 1);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Incorrect arg position", er.what());
    }
    try {
        format("{4096}", 1);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Incorrect arg position", er.what());
    }
    try {
        format("{18446744073709551617000}", 1);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Incorrect arg position", er.what());
    }
}

TEST_F(TestFormat, less_places) {
    try {
        format("{1}+{0} = {1}", 2, "one", "three");
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Incorrect elements count", er.what());
    }
    try {
        format("{0} {2}", 0, 1, 2);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Incorrect elements count", er.what());
    }
    try {
        format("{1}", 0, 1);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Incorrect elements count", er.what());
    }
}

TEST_F(TestFormat, unfinished_number) {
    try {
        format("{1000", 1);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Not finished brackets", er.what());
    }
}

TEST_F(TestFormat, empty_brackets) {
    try {
        format("{}", 1);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Empty brackets", er.what());
    }
    try {
        format("{0} + {}", 1);
        FAIL();
    } catch (const format_error& er) {
        ASSERT_STREQ("Empty brackets", er.what());
    }
}
