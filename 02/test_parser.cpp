#include <gtest/gtest.h>
#include "token_parser.hpp"


class TestParser: public testing::Test {
 protected:
    TokenParser* parser;
    int string_counter;
    int digit_counter;
    int end_counter;
    int start_counter;

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
    parser->SetStartCallback([&]() {
        ++start_counter;
        ASSERT_EQ(0, string_counter);
        ASSERT_EQ(0, digit_counter);
        ASSERT_EQ(0, end_counter);
    });
    parser->SetEndCallback([&]() {
        ++end_counter;
    });
    parser->SetDigitTokenCallback([&](uint64_t) {
        ++digit_counter;
    });
    parser->SetStringTokenCallback([&](const std::string&) {
        ++string_counter;
    });
    parser->Parse("      \t  \n     \t    ");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(0, digit_counter);
    ASSERT_EQ(0, string_counter);
}

TEST_F(TestParser, single_token) {
    parser->SetStartCallback([&]() {
        ++start_counter;
    });
    parser->SetEndCallback([&]() {
        ++end_counter;
    });
    parser->SetDigitTokenCallback([&](uint64_t x) {
        ++digit_counter;
        if (digit_counter == 1) {
            ASSERT_EQ(1, x);
        }
    });
    parser->SetStringTokenCallback([&](const std::string& s) {
        ++string_counter;
        if (string_counter == 1) {
            ASSERT_EQ("a", s);
        }
    });
    parser->Parse("a     ");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(0, digit_counter);
    ASSERT_EQ(1, string_counter);
    parser->Parse("   \t1");
    ASSERT_EQ(2, start_counter);
    ASSERT_EQ(2, end_counter);
    ASSERT_EQ(1, digit_counter);
    ASSERT_EQ(1, string_counter);
}

TEST_F(TestParser, empty_string) {
    parser->SetStartCallback([&]() {
        ++start_counter;
        ASSERT_EQ(0, string_counter);
        ASSERT_EQ(0, digit_counter);
        ASSERT_EQ(0, end_counter);
    });
    parser->SetEndCallback([&]() {
        ++end_counter;
    });
    parser->SetDigitTokenCallback([&](uint64_t) {
        ++digit_counter;
    });
    parser->SetStringTokenCallback([&](const std::string&) {
        ++string_counter;
    });
    parser->Parse("");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(0, digit_counter);
    ASSERT_EQ(0, string_counter);
}

TEST_F(TestParser, mixed_token) {
    parser->SetStartCallback([&]() {
        ++start_counter;
        ASSERT_EQ(0, string_counter);
        ASSERT_EQ(0, digit_counter);
        ASSERT_EQ(0, end_counter);
    });
    parser->SetEndCallback([&]() {
        ++end_counter;
    });
    parser->SetDigitTokenCallback([&](uint64_t x) {
        ++digit_counter;
    });
    parser->SetStringTokenCallback([&](const std::string& s) {
        ++string_counter;
        ASSERT_EQ(0, end_counter);
        if (string_counter == 1) {
            ASSERT_EQ("42kffsdkdfsdk", s);
        }
    });
    parser->Parse("42kffsdkdfsdk\t\t\t     \n");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(0, digit_counter);
    ASSERT_EQ(1, string_counter);
}

TEST_F(TestParser, big_integer) {
    parser->SetStartCallback([&]() {
        ++start_counter;
        ASSERT_EQ(0, string_counter);
        ASSERT_EQ(0, digit_counter);
        ASSERT_EQ(0, end_counter);
    });
    parser->SetEndCallback([&]() {
        ++end_counter;
    });
    parser->SetDigitTokenCallback([&](uint64_t x) {
        ++digit_counter;
    });
    parser->SetStringTokenCallback([&](const std::string& s) {
        ++string_counter;
        ASSERT_EQ(0, end_counter);
        if (string_counter == 1) {
            ASSERT_EQ("1111111111111111111111111111111111111111111111111", s);
        }
    });
    parser->Parse("1111111111111111111111111111111111111111111111111");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(1, string_counter);
    ASSERT_EQ(0, digit_counter);
}

TEST_F(TestParser, leading_zeros) {
    parser->SetStartCallback([&]() {
        ASSERT_EQ(0, string_counter);
        ASSERT_EQ(0, digit_counter);
        ASSERT_EQ(0, end_counter);
        ++start_counter;
    });
    parser->SetEndCallback([&]() {
        ++end_counter;
    });
    parser->SetDigitTokenCallback([&](uint64_t x) {
        ++digit_counter;
        ASSERT_EQ(0, end_counter);
        if (digit_counter == 1) {
            ASSERT_EQ(1234, x);
            ASSERT_EQ(0, string_counter);
        }
        if (digit_counter == 2) {
            ASSERT_EQ(12, x);
            ASSERT_EQ(0, string_counter);
        }
        if (digit_counter == 3) {
            ASSERT_EQ(45, x);
            ASSERT_EQ(0, string_counter);
        }
    });
    parser->SetStringTokenCallback([&](const std::string& s) {
        ++string_counter;
        ASSERT_EQ(0, end_counter);
        if (string_counter == 1) {
            ASSERT_EQ("daladl", s);
            ASSERT_EQ(3, digit_counter);
        }
    });
    parser->Parse("00001234\n012\t00045\ndaladl");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(1, string_counter);
    ASSERT_EQ(3, digit_counter);
}

TEST_F(TestParser, max_uint64t) {
    parser->SetStartCallback([&]() {
        ASSERT_EQ(0, string_counter);
        ASSERT_EQ(0, digit_counter);
        ASSERT_EQ(0, end_counter);
        ++start_counter;
    });
    parser->SetEndCallback([&]() {
        ++end_counter;
    });
    parser->SetDigitTokenCallback([&](uint64_t x) {
        ++digit_counter;
        ASSERT_EQ(0, end_counter);
        if (digit_counter == 1) {
            ASSERT_EQ(18446744073709551615ULL, x);
        }
    });
    parser->SetStringTokenCallback([&](const std::string& s) {
        ++string_counter;
    });
    parser->Parse("18446744073709551615");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(0, string_counter);
    ASSERT_EQ(1, digit_counter);
}

TEST_F(TestParser, max_uint64t_plus_one) {
    parser->SetStartCallback([&]() {
        ++start_counter;
        ASSERT_EQ(0, string_counter);
        ASSERT_EQ(0, digit_counter);
        ASSERT_EQ(0, end_counter);
    });
    parser->SetEndCallback([&]() {
        ++end_counter;
    });
    parser->SetDigitTokenCallback([&](uint64_t x) {
        ++digit_counter;
    });
    parser->SetStringTokenCallback([&](const std::string& s) {
        ++string_counter;
        ASSERT_EQ(0, end_counter);
        if (string_counter == 1) {
            ASSERT_EQ("18446744073709551616", s);
        }
    });
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
    parser->Parse("lol\t123\tdkskd\t 424\t sdkdfk \n44 44 akk");
    ASSERT_EQ(0, start_counter);
    ASSERT_EQ(0, end_counter);
    ASSERT_EQ(0, string_counter);
    ASSERT_EQ(0, digit_counter);
}

TEST_F(TestParser, skip_start) {
    parser->SetEndCallback([&]() {
        ++end_counter;
    });
    parser->SetDigitTokenCallback([&](uint64_t x) {
        ++digit_counter;
        ASSERT_EQ(0, end_counter);
        if (digit_counter == 1) {
            ASSERT_EQ(1, string_counter);
            ASSERT_EQ(123, x);
        }
        if (digit_counter == 2) {
            ASSERT_EQ(2, string_counter);
            ASSERT_EQ(424, x);
        }
        if (digit_counter == 3) {
            ASSERT_EQ(3, string_counter);
            ASSERT_EQ(44, x);
        }
        if (digit_counter == 4) {
            ASSERT_EQ(3, string_counter);
            ASSERT_EQ(44, x);
        }
    });
    parser->SetStringTokenCallback([&](const std::string& s) {
        ++string_counter;
        ASSERT_EQ(0, end_counter);
        if (string_counter == 1) {
            ASSERT_EQ(0, digit_counter);
            ASSERT_EQ("lol", s);
        }
        if (string_counter == 2) {
            ASSERT_EQ(1, digit_counter);
            ASSERT_EQ("dkskd", s);
        }
        if (string_counter == 3) {
            ASSERT_EQ(2, digit_counter);
            ASSERT_EQ("sdkdfk", s);
        }
        if (string_counter == 4) {
            ASSERT_EQ(4, digit_counter);
            ASSERT_EQ("akk", s);
        }
    });
    parser->Parse("lol 123\t dkskd\n 424    sdkdfk 44 44 akk");
    ASSERT_EQ(0, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(4, string_counter);
    ASSERT_EQ(4, digit_counter);
}

TEST_F(TestParser, skip_end) {
    parser->SetStartCallback([&]() {
        ++start_counter;
        ASSERT_EQ(0, string_counter);
        ASSERT_EQ(0, digit_counter);
    });
    parser->SetDigitTokenCallback([&](uint64_t x) {
        ++digit_counter;
        if (digit_counter == 1) {
            ASSERT_EQ(1, string_counter);
            ASSERT_EQ(123, x);
        }
        if (digit_counter == 2) {
            ASSERT_EQ(2, string_counter);
            ASSERT_EQ(424, x);
        }
        if (digit_counter == 3) {
            ASSERT_EQ(3, string_counter);
            ASSERT_EQ(44, x);
        }
        if (digit_counter == 4) {
            ASSERT_EQ(3, string_counter);
            ASSERT_EQ(44, x);
        }
    });
    parser->SetStringTokenCallback([&](const std::string& s) {
        ++string_counter;
        if (string_counter == 1) {
            ASSERT_EQ(0, digit_counter);
            ASSERT_EQ("lol", s);
        }
        if (string_counter == 2) {
            ASSERT_EQ(1, digit_counter);
            ASSERT_EQ("dkskd", s);
        }
        if (string_counter == 3) {
            ASSERT_EQ(2, digit_counter);
            ASSERT_EQ("sdkdfk", s);
        }
        if (string_counter == 4) {
            ASSERT_EQ(4, digit_counter);
            ASSERT_EQ("akk", s);
        }
    });
    parser->Parse("lol\t\t 123 dkskd 424 sdkdfk 44 44 akk");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(0, end_counter);
    ASSERT_EQ(4, string_counter);
    ASSERT_EQ(4, digit_counter);
}

TEST_F(TestParser, skip_digit) {
    parser->SetStartCallback([&]() {
        ++start_counter;
        ASSERT_EQ(0, string_counter);
        ASSERT_EQ(0, end_counter);
    });
    parser->SetEndCallback([&]() {
        ++end_counter;
    });
    parser->SetStringTokenCallback([&](const std::string& s) {
        ++string_counter;
        ASSERT_EQ(0, end_counter);
        if (string_counter == 1) {
            ASSERT_EQ("lol", s);
        }
        if (string_counter == 2) {
            ASSERT_EQ("dkskd", s);
        }
        if (string_counter == 3) {
            ASSERT_EQ("sdkdfk", s);
        }
        if (string_counter == 4) {
            ASSERT_EQ("akk", s);
        }
    });
    parser->Parse("lol 123 dkskd 424 \n\nsdkdfk 44 44 akk");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(4, string_counter);
    ASSERT_EQ(0, digit_counter);
}

TEST_F(TestParser, skip_string) {
    parser->SetStartCallback([&]() {
        ++start_counter;
        ASSERT_EQ(0, end_counter);
        ASSERT_EQ(0, digit_counter);
    });
    parser->SetEndCallback([&]() {
        ++end_counter;
    });
    parser->SetDigitTokenCallback([&](uint64_t x) {
        ++digit_counter;
        ASSERT_EQ(0, end_counter);
        if (digit_counter == 1) {
            ASSERT_EQ(123, x);
        }
        if (digit_counter == 2) {
            ASSERT_EQ(424, x);
        }
        if (digit_counter == 3) {
            ASSERT_EQ(44, x);
        }
        if (digit_counter == 4) {
            ASSERT_EQ(44, x);
        }
    });
    parser->Parse("lol 123           dkskd 424 sdkdfk 44 44 akk");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(0, string_counter);
    ASSERT_EQ(4, digit_counter);
}

TEST_F(TestParser, simple_test) {
    parser->SetStartCallback([&]() {
        ++start_counter;
        ASSERT_EQ(0, string_counter);
        ASSERT_EQ(0, digit_counter);
        ASSERT_EQ(0, end_counter);
    });
    parser->SetEndCallback([&]() {
        ++end_counter;
    });
    parser->SetDigitTokenCallback([&](uint64_t x) {
        ++digit_counter;
        ASSERT_EQ(0, end_counter);
        if (digit_counter == 1) {
            ASSERT_EQ(1, string_counter);
            ASSERT_EQ(123, x);
        }
        if (digit_counter == 2) {
            ASSERT_EQ(2, string_counter);
            ASSERT_EQ(424, x);
        }
        if (digit_counter == 3) {
            ASSERT_EQ(3, string_counter);
            ASSERT_EQ(44, x);
        }
        if (digit_counter == 4) {
            ASSERT_EQ(3, string_counter);
            ASSERT_EQ(44, x);
        }
    });
    parser->SetStringTokenCallback([&](const std::string& s) {
        ++string_counter;
        ASSERT_EQ(0, end_counter);
        if (string_counter == 1) {
            ASSERT_EQ(0, digit_counter);
            ASSERT_EQ("lol", s);
        }
        if (string_counter == 2) {
            ASSERT_EQ(1, digit_counter);
            ASSERT_EQ("dkskd", s);
        }
        if (string_counter == 3) {
            ASSERT_EQ(2, digit_counter);
            ASSERT_EQ("sdkdfk", s);
        }
        if (string_counter == 4) {
            ASSERT_EQ(4, digit_counter);
            ASSERT_EQ("akk", s);
        }
    });
    parser->Parse("lol 123   \n dkskd 424 \t sdkdfk 44 44 akk");
    ASSERT_EQ(1, start_counter);
    ASSERT_EQ(1, end_counter);
    ASSERT_EQ(4, string_counter);
    ASSERT_EQ(4, digit_counter);
}

TEST_F(TestParser, no_handlers) {
    parser->Parse("lol 2342 \ndkdkf 42442 rkdskf 7k dksdkdk 00dsmdk 00sdmsdks 234");
    ASSERT_EQ(0, start_counter);
    ASSERT_EQ(0, end_counter);
    ASSERT_EQ(0, string_counter);
    ASSERT_EQ(0, digit_counter);
}

TEST_F(TestParser, multiple_parse) {
    parser->SetStartCallback([&]() {
        ++start_counter;
        if (start_counter == 1) {
            ASSERT_EQ(0, string_counter);
            ASSERT_EQ(0, digit_counter);
            ASSERT_EQ(0, end_counter);
        }
        if (start_counter == 2) {
            ASSERT_EQ(1, end_counter);
            ASSERT_EQ(2, digit_counter);
            ASSERT_EQ(3, string_counter);
        }
        if (start_counter == 3) {
            ASSERT_EQ(2, end_counter);
            ASSERT_EQ(7, digit_counter);
            ASSERT_EQ(9, string_counter);
        }
    });
    parser->SetEndCallback([&]() {
        ++end_counter;
    });
    parser->SetDigitTokenCallback([&](uint64_t x) {
        ++digit_counter;
        if (digit_counter <= 2) {
            ASSERT_EQ(0, end_counter);
        } else if (digit_counter <= 7) {
            ASSERT_EQ(1, end_counter);
        } else if (digit_counter == 8) {
            ASSERT_EQ(2, end_counter);
        }
        if (digit_counter == 1) {
            ASSERT_EQ(2, string_counter);
            ASSERT_EQ(123, x);
        }
        if (digit_counter == 2) {
            ASSERT_EQ(3, string_counter);
            ASSERT_EQ(11111111, x);
        }
        if (digit_counter == 3) {
            ASSERT_EQ(4, string_counter);
            ASSERT_EQ(323293, x);
        }
        if (digit_counter == 4) {
            ASSERT_EQ(6, string_counter);
            ASSERT_EQ(43232, x);
        }
        if (digit_counter == 5) {
            ASSERT_EQ(6, string_counter);
            ASSERT_EQ(42424, x);
        }
        if (digit_counter == 6) {
            ASSERT_EQ(6, string_counter);
            ASSERT_EQ(453, x);
        }
        if (digit_counter == 7) {
            ASSERT_EQ(9, string_counter);
            ASSERT_EQ(12, x);
        }
        if (digit_counter == 8) {
            ASSERT_EQ(9, string_counter);
            ASSERT_EQ(123, x);
        }
    });
    parser->SetStringTokenCallback([&](const std::string& s) {
        ++string_counter;
        if (string_counter <= 3) {
            ASSERT_EQ(0, end_counter);
        } else if (string_counter <= 9) {
            ASSERT_EQ(1, end_counter);
        } else if (start_counter == 10) {
            ASSERT_EQ(2, end_counter);
        }
        if (string_counter == 1) {
            ASSERT_EQ(0, digit_counter);
            ASSERT_EQ("llll", s);
        }
        if (string_counter == 2) {
            ASSERT_EQ(0, digit_counter);
            ASSERT_EQ("kkkkk", s);
        }
        if (string_counter == 3) {
            ASSERT_EQ(1, digit_counter);
            ASSERT_EQ("slsals", s);
        }
        if (string_counter == 4) {
            ASSERT_EQ(2, digit_counter);
            ASSERT_EQ("dskfsdkld", s);
        }
        if (string_counter == 5) {
            ASSERT_EQ(3, digit_counter);
            ASSERT_EQ("dsfksdk", s);
        }
        if (string_counter == 6) {
            ASSERT_EQ(3, digit_counter);
            ASSERT_EQ("kdksfs", s);
        }
        if (string_counter == 7) {
            ASSERT_EQ(6, digit_counter);
            ASSERT_EQ("dkskds", s);
        }
        if (string_counter == 8) {
            ASSERT_EQ(6, digit_counter);
            ASSERT_EQ("fkksf4", s);
        }
        if (string_counter == 9) {
            ASSERT_EQ(6, digit_counter);
            ASSERT_EQ("o444", s);
        }
        if (string_counter == 10) {
            ASSERT_EQ(8, digit_counter);
            ASSERT_EQ("ddsddsds", s);
        }
    });
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
    parser->Parse("123 \t    ddsddsds");
    ASSERT_EQ(3, start_counter);
    ASSERT_EQ(3, end_counter);
    ASSERT_EQ(8, digit_counter);
    ASSERT_EQ(10, string_counter);
}
