#include <gtest/gtest.h>
#include <sstream>
#include "matrix.hpp"

class TestMatrix: public testing::Test {
 protected:
    void SetUp() {
    }

    void TearDown() {
    }
};

Matrix fill_rectangle_matrix(int n, int m) {
    Matrix res(n, m);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            res[i][j] = (i + 1) * (j + 1);
        }
    }
    return res;
}

Matrix fill_square_matrix(int n) {
    Matrix res(n);
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            res[i][j] = (i + 1) * (j + 1);
        }
    }
    return res;
}


TEST_F(TestMatrix, get_params) {
    Matrix m;
    ASSERT_EQ(0, m.get_rows());
    ASSERT_EQ(0, m.get_cols());
    m = fill_rectangle_matrix(3, 4);
    ASSERT_EQ(3, m.get_rows());
    ASSERT_EQ(4, m.get_cols());
    m = fill_square_matrix(3);
    ASSERT_EQ(3, m.get_rows());
    ASSERT_EQ(3, m.get_cols());
    m = fill_rectangle_matrix(1000, 1200);
    const Matrix a(m);
    ASSERT_EQ(1000, a.get_rows());
    ASSERT_EQ(1200, a.get_cols());
}

TEST_F(TestMatrix, correct_assignment) {
    Matrix m = fill_rectangle_matrix(2, 3);
    ASSERT_EQ(1, m[0][0]);
    ASSERT_EQ(2, m[0][1]);
    ASSERT_EQ(3, m[0][2]);
    ASSERT_EQ(2, m[1][0]);
    ASSERT_EQ(4, m[1][1]);
    ASSERT_EQ(6, m[1][2]);
    m = fill_rectangle_matrix(1000, 1200);
    const Matrix a(m);
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 1200; ++j) {
            ASSERT_EQ((i + 1) * (j + 1), a[i][j]);
        }
    }
}

TEST_F(TestMatrix, incorrect_assignment) {
    Matrix m = fill_rectangle_matrix(3, 4);
    ASSERT_THROW(m[10][0], std::out_of_range);
    ASSERT_THROW(m[0][10], std::out_of_range);
    ASSERT_THROW(m[10][10], std::out_of_range);
    ASSERT_THROW(m[-1][3], std::out_of_range);
    ASSERT_THROW(m[2][-1], std::out_of_range);
    ASSERT_THROW(m[3][3], std::out_of_range);
    ASSERT_THROW(m[2][4], std::out_of_range);
    ASSERT_THROW(m[3][4], std::out_of_range);
    ASSERT_THROW(m[-1][-1], std::out_of_range);
    ASSERT_THROW(m[-1][4], std::out_of_range);
    ASSERT_THROW(m[3][-1], std::out_of_range);
}

TEST_F(TestMatrix, multiply) {
    Matrix m = fill_rectangle_matrix(1000, 1200);
    m *= 2;
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 1200; ++j) {
            ASSERT_EQ(2 * (i + 1) * (j + 1), m[i][j]);
        }
    }
    m *= 3;
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 1200; ++j) {
            ASSERT_EQ(6 * (i + 1) * (j + 1), m[i][j]);
        }
    }
    m *= 0;
    for (int i = 0; i < 1000; ++i) {
        for (int j = 0; j < 1200; ++j) {
            ASSERT_EQ(0, m[i][j]);
        }
    }
}

TEST_F(TestMatrix, printing) {
    std::stringstream s;
    Matrix m = fill_square_matrix(1);
    s << m;
    ASSERT_EQ("1 \n", s.str());
    s.str("");
    m = fill_rectangle_matrix(2, 3);
    s << m;
    ASSERT_EQ("1 2 3 \n2 4 6 \n", s.str());
}

TEST_F(TestMatrix, comparison) {
    Matrix m;
    Matrix m2;
    ASSERT_TRUE(m == m2);
    ASSERT_FALSE(m != m2);
    m = fill_rectangle_matrix(3, 3);
    ASSERT_FALSE(m == m2);
    ASSERT_TRUE(m != m2);
    m2 = fill_square_matrix(1);
    ASSERT_FALSE(m == m2);
    ASSERT_TRUE(m != m2);
    m2 = fill_square_matrix(3);
    ASSERT_TRUE(m == m2);
    ASSERT_FALSE(m != m2);
    m[0][0] = 0;
    ASSERT_FALSE(m == m2);
    ASSERT_TRUE(m != m2);
}

TEST_F(TestMatrix, add) {
    Matrix m = fill_rectangle_matrix(3, 10);
    Matrix m2 = fill_rectangle_matrix(3, 10);
    Matrix m3 = fill_rectangle_matrix(3, 10);
    m3 *= 2;
    ASSERT_EQ(m3, m + m2);
    m = fill_rectangle_matrix(3, 3);
    ASSERT_THROW(m + m2, std::out_of_range);
    m = fill_rectangle_matrix(2, 10);
    ASSERT_THROW(m + m2, std::out_of_range);
    m = fill_rectangle_matrix(10, 10);
    ASSERT_THROW(m + m2, std::out_of_range);
    m3 *= 0;
    ASSERT_EQ(m2 + m3, m2);
}

TEST_F(TestMatrix, multiple_operations) {
    Matrix m1 = fill_rectangle_matrix(3, 3);
    const Matrix m2 = fill_square_matrix(3);
    Matrix m3 = m1 + m2;
    m3 *= 2;
    m1 *= 3;
    (m3 += m1) *= -1;
    m3[0][0] = 10000;
    std::stringstream s;
    s << m3;
    ASSERT_EQ("10000 -14 -21 \n-14 -28 -42 \n-21 -42 -63 \n", s.str());
}

TEST_F(TestMatrix, number) {
    Matrix m1 = fill_square_matrix(1);
    Matrix m2 = fill_square_matrix(1);
    m2[0][0] = 3;
    ASSERT_TRUE(m1 != m2);
    ASSERT_EQ(m2, m1 + m1 + m1);
    m1 *= 3;
    ASSERT_EQ(m2, m1);
    ASSERT_TRUE(m1 == m2);
    ASSERT_EQ(1, m1.get_cols());
    ASSERT_EQ(1, m1.get_rows());
}

TEST_F(TestMatrix, vector) {
    Matrix m1 = fill_rectangle_matrix(10, 1);
    Matrix m2 = fill_rectangle_matrix(10, 1);
    m2 *= 3;
    ASSERT_TRUE(m1 != m2);
    ASSERT_EQ(m2, m1 + m1 + m1);
    m1 *= 3;
    ASSERT_EQ(m2, m1);
    ASSERT_TRUE(m1 == m2);
    ASSERT_EQ(1, m1.get_cols());
    ASSERT_EQ(10, m1.get_rows());
}

TEST_F(TestMatrix, transposed_vector) {
    Matrix m1 = fill_rectangle_matrix(1, 10);
    Matrix m2 = fill_rectangle_matrix(1, 10);
    m2 *= 3;
    ASSERT_TRUE(m1 != m2);
    ASSERT_EQ(m2, m1 + m1 + m1);
    m1 *= 3;
    ASSERT_EQ(m2, m1);
    ASSERT_TRUE(m1 == m2);
    ASSERT_EQ(10, m1.get_cols());
    ASSERT_EQ(1, m1.get_rows());
}