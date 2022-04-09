#include <gtest/gtest.h>
#include "token_parser.hpp"

int string_counter = 0;
int digit_counter = 0;
int end_counter = 0;
int start_counter = 0;

int start() {
    ++start_counter;
    return 0;
}

int end() {
    ++end_counter;
    return 0;
}

int str(const std::string&) {
    ++string_counter;
    return 0;
}

int digit(uint64_t) {
    ++digit_counter;
    return 0;
}

int str_check_sequence(const std::string& s) {
    static bool flag = true;
    static int counter = 0;
    if (counter > 2) {
        return flag;
    }
    if (counter == 0 && s != "lol") {
        flag = false;
    }
    if (counter == 1 && s != "kek") {
        flag = false;
    }
    if (counter == 2 && s != "abc") {
        flag = false;
    }
    ++counter;
    return flag;
}

int digit_check_sequence(uint64_t x) {
    static bool flag = true;
    static int counter = 0;
    if (counter > 2) {
        return flag;
    }
    if (counter == 0 && x != 42) {
        flag = false;
    }
    if (counter == 1 && x != 0) {
        flag = false;
    }
    if (counter == 2 && x != 37) {
        flag = false;
    }
    ++counter;
    return flag;
}


class TestParser: public testing::Test {
 protected:
    TokenParser* parser;

    void SetUp() {
        parser = new TokenParser;
        string_counter = 0;
        digit_counter = 0;
        end_counter = 0;
        start_counter = 0;
    }

    void TearDown() {
        delete parser;
    }
};

TEST_F(TestParser, only_blanks) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("                 ");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(0, digit_counter);
    ASSERT_EQ(0, string_counter);
}

TEST_F(TestParser, single_token) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("a");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(0, digit_counter);
    ASSERT_EQ(1, string_counter);
    parser->Parse("1");
    ASSERT_EQ(2, start_counter);
    ASSERT_EQ(2, end_counter);
    ASSERT_EQ(1, digit_counter);
    ASSERT_EQ(1, string_counter);
}

TEST_F(TestParser, empty_string) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(0, digit_counter);
    ASSERT_EQ(0, string_counter);
}

TEST_F(TestParser, mixed_token) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("42kffsdkdfsdk");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(0, digit_counter);
    ASSERT_EQ(1, string_counter);
}

TEST_F(TestParser, big_integer) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("1111111111111111111111111111111111111111111111111");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(1, string_counter);
    ASSERT_EQ(0, digit_counter);
}

TEST_F(TestParser, leading_zeros) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("00001234 012 00045 daladl");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(1, string_counter);
    ASSERT_EQ(3, digit_counter);
}

TEST_F(TestParser, max_uint64t) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("18446744073709551615");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(0, string_counter);
    ASSERT_EQ(1, digit_counter);
}

TEST_F(TestParser, max_uint64t_plus_one) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("18446744073709551616");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(1, string_counter);
    ASSERT_EQ(0, digit_counter);
}

TEST_F(TestParser, set_nullptrs) {
    parser->SetStartCallback(nullptr);
    parser->SetEndCallback(nullptr);
    parser->SetStringTokenCallback(nullptr);
    parser->SetDigitTokenCallback(nullptr);
    parser->Parse("lol 123 dkskd 424 sdkdfk 44 44 akk");
    ASSERT_EQ(0, start_counter);
    ASSERT_EQ(0, end_counter);
    ASSERT_EQ(0, string_counter);
    ASSERT_EQ(0, digit_counter);
}

TEST_F(TestParser, skip_start) {
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("lol 123 dkskd 424 sdkdfk 44 44 akk");
    ASSERT_EQ(0, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(4, string_counter);
    ASSERT_EQ(4, digit_counter);
}

TEST_F(TestParser, skip_end) {
    parser->SetStartCallback(start);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("lol 123 dkskd 424 sdkdfk 44 44 akk");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(0, end_counter);
    ASSERT_EQ(4, string_counter);
    ASSERT_EQ(4, digit_counter);
}

TEST_F(TestParser, skip_digit) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetStringTokenCallback(str);
    parser->Parse("lol 123 dkskd 424 sdkdfk 44 44 akk");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(4, string_counter);
    ASSERT_EQ(0, digit_counter);
}

TEST_F(TestParser, skip_string) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->Parse("lol 123 dkskd 424 sdkdfk 44 44 akk");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(0, string_counter);
    ASSERT_EQ(4, digit_counter);
}

TEST_F(TestParser, simple_test) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("lol 123 dkskd 424 sdkdfk 44 44 akk");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(4, string_counter);
    ASSERT_EQ(4, digit_counter);
}

TEST_F(TestParser, no_handlers) {
    parser->Parse("lol 2342 dkdkf 42442 rkdskf 7k dksdkdk 00dsmdk 00sdmsdks 234");
    ASSERT_EQ(0, start_counter);
    ASSERT_EQ(0, end_counter);
    ASSERT_EQ(0, string_counter);
    ASSERT_EQ(0, digit_counter);
}

TEST_F(TestParser, multiple_parse) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("llll kkkkk 123 slsals 11111111");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(2, digit_counter);
    ASSERT_EQ(3, string_counter);
    parser->Parse("dskfsdkld 323293 dsfksdk kdksfs 43232 42424 453 dkskds fkksf4 o444 000012");
    ASSERT_EQ(2, start_counter);
    ASSERT_EQ(2, end_counter);
    ASSERT_EQ(7, digit_counter);
    ASSERT_EQ(9, string_counter);
    parser->Parse("123 ddsddsds");
    ASSERT_EQ(3, start_counter);
    ASSERT_EQ(3, end_counter);
    ASSERT_EQ(8, digit_counter);
    ASSERT_EQ(10, string_counter);
}


TEST_F(TestParser, check_sequence) {
    parser->SetStringTokenCallback(str_check_sequence);
    parser->SetDigitTokenCallback(digit_check_sequence);
    parser->Parse("lol 42 0 kek 37 abc");
    ASSERT_TRUE(str_check_sequence(""));
    ASSERT_TRUE(digit_check_sequence(0));
}
