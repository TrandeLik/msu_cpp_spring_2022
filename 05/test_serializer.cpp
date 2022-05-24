#include <gtest/gtest.h>
#include "serializer.hpp"
#include "deserializer.hpp"

class TestSerializer: public testing::Test {
protected:
    void SetUp() {
    }

    void TearDown() {
    }
};

struct SimpleData {
    uint64_t a;
    bool b;
    uint64_t c;
    template <class Serializer>
    Error serialize(Serializer& serializer) {
        return serializer(a, b, c);
    }
};

struct OneBool {
    bool a;
    template <class Serializer>
    Error serialize(Serializer& serializer) {
        return serializer(a);
    }
};

struct OneInt {
    uint64_t a;
    template <class Serializer>
    Error serialize(Serializer& serializer) {
        return serializer(a);
    }
};

struct OneDouble {
    double a;
    template <class Serializer>
    Error serialize(Serializer& serializer) {
        return serializer(a);
    }
};

struct DataWithDouble {
    uint64_t a;
    double f;
    bool b;
    template <class Serializer>
    Error serialize(Serializer& serializer) {
        return serializer(a, f, b);
    }
};


TEST_F(TestSerializer, simple_test) {
    SimpleData x{ 1, true, 2 };
    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);
    SimpleData y{ 0, false, 0 };
    Deserializer deserializer(stream);
    const Error err = deserializer.load(y);
    ASSERT_EQ(err, Error::NoError);
    ASSERT_EQ(x.a, y.a);
    ASSERT_EQ(x.b, y.b);
    ASSERT_EQ(x.c, y.c);
}

TEST_F(TestSerializer, one_bool_test) {
    OneBool x{false};
    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);
    OneBool y{true};
    Deserializer deserializer(stream);
    const Error err = deserializer.load(y);
    ASSERT_EQ(err, Error::NoError);
    ASSERT_EQ(x.a, y.a);
}

TEST_F(TestSerializer, one_int_test) {
    OneInt x{1000};
    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);
    OneInt y{2000};
    Deserializer deserializer(stream);
    const Error err = deserializer.load(y);
    ASSERT_EQ(err, Error::NoError);
    ASSERT_EQ(x.a, y.a);
}

TEST_F(TestSerializer, one_double_test) {
    OneDouble x{1000};
    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);
    OneDouble y{2000};
    Deserializer deserializer(stream);
    const Error err = deserializer.load(y);
    ASSERT_EQ(err, Error::CorruptedArchive);
}

TEST_F(TestSerializer, data_with_double) {
    DataWithDouble x{10000, 0.1, true};
    std::stringstream stream;
    Serializer serializer(stream);
    serializer.save(x);
    DataWithDouble y{20000, -0.1, false};
    Deserializer deserializer(stream);
    const Error err = deserializer.load(y);
    ASSERT_EQ(err, Error::CorruptedArchive);
}

TEST_F(TestSerializer, too_big_int) {
    std::stringstream stream;
    stream << "1111111111111111111111111111111111111111111111 true 1";
    SimpleData data;
    Deserializer deserializer(stream);
    const Error err = deserializer.load(data);
    ASSERT_EQ(err, Error::CorruptedArchive);
}

TEST_F(TestSerializer, non_bool) {
    std::stringstream stream;
    stream << "1111111111111111 tfue 1";
    SimpleData data;
    Deserializer deserializer(stream);
    const Error err = deserializer.load(data);
    ASSERT_EQ(err, Error::CorruptedArchive);
}

TEST_F(TestSerializer, wrong_order) {
    std::stringstream stream;
    stream << "true 111 1";
    SimpleData data;
    Deserializer deserializer(stream);
    const Error err = deserializer.load(data);
    ASSERT_EQ(err, Error::CorruptedArchive);
}

TEST_F(TestSerializer, less_elements) {
    std::stringstream stream;
    stream << "111 true";
    SimpleData data;
    Deserializer deserializer(stream);
    const Error err = deserializer.load(data);
    ASSERT_EQ(err, Error::CorruptedArchive);
}

TEST_F(TestSerializer, max_int) {
    std::stringstream stream;
    stream << "18446744073709551615 true 1";
    SimpleData data;
    Deserializer deserializer(stream);
    const Error err = deserializer.load(data);
    ASSERT_EQ(err, Error::NoError);
    ASSERT_EQ(data.a, 18446744073709551615ull);
    ASSERT_EQ(data.b, true);
    ASSERT_EQ(data.c, 1);
}
