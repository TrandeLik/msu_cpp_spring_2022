#include <gtest/gtest.h>
#include <unistd.h>
#include "thread_pool.hpp"

struct A {};

void f1(const A&) {
    sleep(1);
}

int f2() {
    return 0;
}

double f3(int) {
    sleep(2);
    return 1.25;
}

int f4() {
    return 1;
}

char* f5(size_t) {
    sleep(1);
    return nullptr;
}

class TestPool: public testing::Test {
 protected:
    void SetUp() {
    }

    void TearDown() {
    }
};

void run_test(std::size_t capacity) {
    ThreadPool pool(capacity);
    auto task1 = pool.exec(f1, A());
    auto task2 = pool.exec(f2);
    auto task3 = pool.exec(f3, 0);
    auto task4 = pool.exec(f4);
    auto task5 = pool.exec(f5, 2);
    task1.get();
    ASSERT_EQ(0, task2.get());
    ASSERT_EQ(1.25, task3.get());
    ASSERT_EQ(1, task4.get());
    ASSERT_EQ(static_cast<char*>(nullptr), task5.get());
}

TEST_F(TestPool, small_capacity) {
    run_test(3);
}

TEST_F(TestPool, enough_capacity) {
    run_test(5);
}

TEST_F(TestPool, big_capacity) {
    run_test(8);
}

TEST_F(TestPool, only_one_thread) {
    run_test(1);
}
