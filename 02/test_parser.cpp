#include <gtest/gtest.h>
#include "test_parser.hpp"

TEST_F(TestParser, empty_string) {
    parser->SetStartCallback(*start);
    parser->SetEndCallback(*end);
    parser->SetDigitTokenCallback(*digit);
    parser->SetStringTokenCallback(*str);
    parser->Parse("");
    ASSERT_EQ(1, parser->start_counter);
    ASSERT_EQ(1, parser->end_counter);
    ASSERT_EQ(0, parser->digit_counter);
    ASSERT_EQ(0, parser->string_counter);
}

TEST_F(TestParser, big_integer) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("1111111111111111111111111111111111111111111111111");
    ASSERT_EQ(1, parser->start_counter);
    ASSERT_EQ(1, parser->end_counter);
    ASSERT_EQ(1, parser->string_counter);
    ASSERT_EQ(0, parser->digit_counter);
}

TEST_F(TestParser, leading_zeros) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("00001234 012 00045 daladl");
    ASSERT_EQ(1, parser->start_counter);
    ASSERT_EQ(1, parser->end_counter);
    ASSERT_EQ(1, parser->string_counter);
    ASSERT_EQ(3, parser->digit_counter);
}

TEST_F(TestParser, max_uint64t) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("18446744073709551615");
    ASSERT_EQ(1, parser->start_counter);
    ASSERT_EQ(1, parser->end_counter);
    ASSERT_EQ(0, parser->string_counter);
    ASSERT_EQ(1, parser->digit_counter);
}

TEST_F(TestParser, skip_start) {
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("lol 123 dkskd 424 sdkdfk 44 44 akk");
    ASSERT_EQ(0, parser->start_counter);
    ASSERT_EQ(1, parser->end_counter);
    ASSERT_EQ(4, parser->string_counter);
    ASSERT_EQ(4, parser->digit_counter);
}

TEST_F(TestParser, skip_end) {
    parser->SetStartCallback(start);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("lol 123 dkskd 424 sdkdfk 44 44 akk");
    ASSERT_EQ(1, parser->start_counter);
    ASSERT_EQ(0, parser->end_counter);
    ASSERT_EQ(4, parser->string_counter);
    ASSERT_EQ(4, parser->digit_counter);
}

TEST_F(TestParser, skip_digit) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetStringTokenCallback(str);
    parser->Parse("lol 123 dkskd 424 sdkdfk 44 44 akk");
    ASSERT_EQ(1, parser->start_counter);
    ASSERT_EQ(1, parser->end_counter);
    ASSERT_EQ(4, parser->string_counter);
    ASSERT_EQ(0, parser->digit_counter);
}

TEST_F(TestParser, skip_string) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->Parse("lol 123 dkskd 424 sdkdfk 44 44 akk");
    ASSERT_EQ(1, parser->start_counter);
    ASSERT_EQ(1, parser->end_counter);
    ASSERT_EQ(0, parser->string_counter);
    ASSERT_EQ(4, parser->digit_counter);
}

TEST_F(TestParser, simple_test) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("lol 123 dkskd 424 sdkdfk 44 44 akk");
    ASSERT_EQ(1, parser->start_counter);
    ASSERT_EQ(1, parser->end_counter);
    ASSERT_EQ(4, parser->string_counter);
    ASSERT_EQ(4, parser->digit_counter);
}

TEST_F(TestParser, no_handlers) {
    parser->Parse("lol 2342 dkdkf 42442 rkdskf 7k dksdkdk 00dsmdk 00sdmsdks 234");
    ASSERT_EQ(0, parser->start_counter);
    ASSERT_EQ(0, parser->end_counter);
    ASSERT_EQ(0, parser->string_counter);
    ASSERT_EQ(0, parser->digit_counter);
}

TEST_F(TestParser, multiple_parse) {
    parser->SetStartCallback(start);
    parser->SetEndCallback(end);
    parser->SetDigitTokenCallback(digit);
    parser->SetStringTokenCallback(str);
    parser->Parse("llll kkkkk 123 slsals 11111111");
    ASSERT_EQ(1, parser->start_counter);
    ASSERT_EQ(1, parser->end_counter);
    ASSERT_EQ(2, parser->digit_counter);
    ASSERT_EQ(3, parser->string_counter);
    parser->Parse("dskfsdkld 323293 dsfksdk kdksfs 43232 42424 453 dkskds fkksf4 o444 000012");
    ASSERT_EQ(2, parser->start_counter);
    ASSERT_EQ(2, parser->end_counter);
    ASSERT_EQ(7, parser->digit_counter);
    ASSERT_EQ(9, parser->string_counter);
    parser->Parse("123 ddsddsds");
    ASSERT_EQ(3, parser->start_counter);
    ASSERT_EQ(3, parser->end_counter);
    ASSERT_EQ(8, parser->digit_counter);
    ASSERT_EQ(10, parser->string_counter);
}
