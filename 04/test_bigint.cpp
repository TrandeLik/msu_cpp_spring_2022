#include <gtest/gtest.h>
#include <sstream>
#include "bigint.hpp"


class TestBigint: public testing::Test {
 protected:
    void SetUp() {
    }

    void TearDown() {
    }
};

TEST_F(TestBigint, simple_test) {
    BigInt a = 1;
    BigInt b("123456789012345678901234567890");
    BigInt c = a * b + 2;
    BigInt d;
    d = std::move(c);
    a = d + b;
    std::stringstream s;
    s << a;
    ASSERT_EQ("246913578024691357802469135782", s.str());
}

TEST_F(TestBigint, addition) {
    BigInt a("1000000");
    BigInt c("-1000");
    BigInt d("-42393232939329");
    BigInt e(0);
    std::stringstream s;
    s << a + 1;
    ASSERT_EQ("1000001", s.str());
    s.str("");
    s << a + c;
    ASSERT_EQ("999000", s.str());
    s.str("");
    s << a + d;
    ASSERT_EQ("-42393231939329", s.str());
    s.str("");
    s << a + e;
    ASSERT_EQ("1000000", s.str());
    s.str("");
    s << c + d;
    ASSERT_EQ("-42393232940329", s.str());
}

TEST_F(TestBigint, subtraction) {
    BigInt a("1000000");
    BigInt c("-1000");
    BigInt d("-42393232939329");
    BigInt e(0);
    std::stringstream s;
    s << a - 1;
    ASSERT_EQ("999999", s.str());
    s.str("");
    s << a - c;
    ASSERT_EQ("1001000", s.str());
    s.str("");
    s << a - d;
    ASSERT_EQ("42393233939329", s.str());
    s.str("");
    s << a - e;
    ASSERT_EQ("1000000", s.str());
    s.str("");
    s << c - d;
    ASSERT_EQ("42393232938329", s.str());
}

TEST_F(TestBigint, multiplication) {
    BigInt a("1000000");
    BigInt c("-1000");
    BigInt d("-42393232939329");
    BigInt e(0);
    std::stringstream s;
    s << a * 1;
    ASSERT_EQ("1000000", s.str());
    s.str("");
    s << a * c;
    ASSERT_EQ("-1000000000", s.str());
    s.str("");
    s << a * d;
    ASSERT_EQ("-42393232939329000000", s.str());
    s.str("");
    s << a * e;
    ASSERT_EQ("0", s.str());
    s.str("");
    s << c * d;
    ASSERT_EQ("42393232939329000", s.str());
}

TEST_F(TestBigint, neg) {
    BigInt a("1000000");
    BigInt c("-1000");
    BigInt d("-42393232939329");
    BigInt e(0);
    std::stringstream s;
    s << -a;
    ASSERT_EQ("-1000000", s.str());
    s.str("");
    s << -c;
    ASSERT_EQ("1000", s.str());
    s.str("");
    s << -d;
    ASSERT_EQ("42393232939329", s.str());
    s.str("");
    s << -e;
    ASSERT_EQ("0", s.str());
}

TEST_F(TestBigint, comparison) {
    BigInt a("1000000000000000000000000");
    BigInt b("999999999999999999999999");
    ASSERT_TRUE(a != b);
    ASSERT_TRUE(a > b);
    ASSERT_TRUE(a >= b);
    ASSERT_TRUE(b < a);
    ASSERT_TRUE(b <= a);
    ASSERT_TRUE(a == b + 1);
    ASSERT_TRUE(a <= b + 1);
    ASSERT_TRUE(a >= b + 1);
    ASSERT_TRUE(-a < b);
    ASSERT_TRUE(a > -b);
    ASSERT_TRUE(-a < -b);
}

TEST_F(TestBigint, assignment) {
    BigInt a("10000");
    BigInt b = std::move(a);
    ASSERT_EQ(b, BigInt("10000"));
    ASSERT_EQ(a, BigInt());
    BigInt c;
    c = std::move(b);
    ASSERT_EQ(c, BigInt("10000"));
    ASSERT_EQ(b, BigInt());
    BigInt d = c;
    ASSERT_EQ(c, BigInt("10000"));
    ASSERT_EQ(d, BigInt("10000"));
}
