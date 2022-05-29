#include <gtest/gtest.h>
#include <random>
#include <fstream>
#include <cstdio>
#include "sort.hpp"

void fill_file(const std::string& filename, std::size_t size) {
    std::ofstream file;
    file.open(filename, std::ios::binary | std::ios::out);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<uint64_t> dis(std::numeric_limits<uint64_t>::min(),
                                            std::numeric_limits<uint64_t>::max());
    for (std::size_t i = 0; i < size; ++i) {
        uint64_t tmp = dis(gen);
        file.write(reinterpret_cast<char*>(&tmp), sizeof(tmp));
    }
    file.close();
}

bool is_sorted(const std::string& filename, std::size_t size) {
    std::ifstream file(filename, std::ios::binary  | std::ios::in);
    auto fsize = file.tellg();
    file.seekg(0, std::ios::end);
    fsize = file.tellg() - fsize;
    if (fsize / sizeof(uint64_t) != size) {
        file.close();
        std::remove(filename.c_str());
        return false;
    }
    file.seekg(0, std::ios::beg);
    uint64_t prev = 0;
    uint64_t cur = 0;
    for (std::size_t i = 0; i < size; ++i) {
        file.read(reinterpret_cast<char*>(&cur), sizeof(cur));
        if (prev > cur) {
            file.close();
            std::remove(filename.c_str());
            return false;
        }
        prev = cur;
    }
    file.close();
    std::remove(filename.c_str());
    return true;
}

class TestSort: public testing::Test {
 protected:
    void SetUp() {
    }

    void TearDown() {
    }
};

TEST_F(TestSort, small_file_even_length) {
    fill_file("input.bin", 10);
    sort("input.bin");
    ASSERT_TRUE(is_sorted("sorted_input.bin", 10));
    std::remove("input.bin");
}

TEST_F(TestSort, small_file_odd_length) {
    fill_file("input.bin", 11);
    sort("input.bin");
    ASSERT_TRUE(is_sorted("sorted_input.bin", 11));
    std::remove("input.bin");
}


TEST_F(TestSort, big_file_even_length) {
    fill_file("input.bin", 10000);
    sort("input.bin");
    ASSERT_TRUE(is_sorted("sorted_input.bin", 10000));
    std::remove("input.bin");
}

TEST_F(TestSort, big_file_odd_length) {
    fill_file("input.bin", 10001);
    sort("input.bin");
    ASSERT_TRUE(is_sorted("sorted_input.bin", 10001));
    std::remove("input.bin");
}

TEST_F(TestSort, huge_file_even_length) {
    fill_file("input.bin", 100000);
    sort("input.bin");
    ASSERT_TRUE(is_sorted("sorted_input.bin", 100000));
    std::remove("input.bin");
}

TEST_F(TestSort, huge_file_odd_length) {
    fill_file("input.bin", 100001);
    sort("input.bin");
    ASSERT_TRUE(is_sorted("sorted_input.bin", 100001));
    std::remove("input.bin");
}

TEST_F(TestSort, very_huge_file_even_length) {
    fill_file("input.bin", 1000000);
    sort("input.bin");
    ASSERT_TRUE(is_sorted("sorted_input.bin", 1000000));
    std::remove("input.bin");
}

TEST_F(TestSort, very_huge_file_odd_length) {
    fill_file("input.bin", 1000001);
    sort("input.bin");
    ASSERT_TRUE(is_sorted("sorted_input.bin", 1000001));
    std::remove("input.bin");
}

TEST_F(TestSort, two_elements_in_file) {
    fill_file("input.bin", 2);
    sort("input.bin");
    ASSERT_TRUE(is_sorted("sorted_input.bin", 2));
    std::remove("input.bin");
}

TEST_F(TestSort, one_element_in_file) {
    fill_file("input.bin", 1);
    sort("input.bin");
    ASSERT_TRUE(is_sorted("sorted_input.bin", 1));
    std::remove("input.bin");
}

TEST_F(TestSort, incorrect_file_size) {
    fill_file("input.bin", 0);
    try {
        sort("input.bin");
        FAIL();
    } catch (const std::invalid_argument& er) {
        ASSERT_STREQ("Invalid file size\n", er.what());
    }
    std::remove("input.bin");
}


