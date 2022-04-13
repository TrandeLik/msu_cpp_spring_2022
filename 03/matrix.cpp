#include "matrix.hpp"
#include <iostream>
#include <cstring>
#include <cstdint>
#include <utility>

Matrix::Matrix(size_t n, size_t m) {
    rows_count = n;
    cols_count = m;
    rows_ = reinterpret_cast<ProxyRow*>(new char[rows_count * sizeof(ProxyRow)]);
    for (size_t i = 0; i < rows_count; ++i) {
        new (rows_ + i) ProxyRow(cols_count);
    }
}

Matrix::Matrix(size_t n) {
    rows_count = n;
    cols_count = n;
    rows_ = reinterpret_cast<ProxyRow*>(new char[rows_count * sizeof(ProxyRow)]);
    for (size_t i = 0; i < rows_count; ++i) {
        new (rows_ + i) ProxyRow(cols_count);
    }
}

Matrix::Matrix(const Matrix& m) {
    rows_count = m.rows_count;
    cols_count = m.cols_count;
    rows_ = reinterpret_cast<ProxyRow*>(new char[rows_count * sizeof(ProxyRow)]);
    for (size_t i = 0; i < rows_count; ++i) {
        new (rows_ + i) ProxyRow(m.rows_[i]);
    }
}

Matrix::~Matrix() {
    if (rows_ != nullptr) {
        for (size_t i = 0; i < rows_count; ++i) {
            (rows_ + i)->~ProxyRow();
        }
        delete[] reinterpret_cast<char *>(rows_);
    }
}

size_t Matrix::get_rows() const {
    return rows_count;
}

size_t Matrix::get_cols() const {
    return cols_count;
}

Matrix::ProxyRow& Matrix::operator[](size_t i) {
    if (i >= rows_count) {
        throw std::out_of_range("");
    }
    return rows_[i];
}

Matrix::ProxyRow Matrix::operator[](size_t i) const {
    if (i >= rows_count) {
        throw std::out_of_range("");
    }
    return rows_[i];
}

Matrix& Matrix::operator=(Matrix m) {
    std::swap(rows_count, m.rows_count);
    std::swap(cols_count, m.cols_count);
    std::swap(rows_, m.rows_);
    return *this;
}

Matrix& Matrix::operator*=(int32_t x) {
    for (size_t i = 0; i < rows_count; ++i) {
        rows_[i] *= x;
    }
    return *this;
}

Matrix& Matrix::operator+=(const Matrix& m) {
    if (m.rows_count != rows_count || m.cols_count != cols_count) {
        throw std::out_of_range("");
    }
    for (size_t i = 0; i < rows_count; ++i) {
        rows_[i] += m.rows_[i];
    }
    return *this;
}

Matrix operator+(const Matrix& m1, const Matrix& m2) {
    Matrix tmp(m1);
    tmp += m2;
    return tmp;
}

bool Matrix::operator==(const Matrix& m) const {
    if (cols_count != m.cols_count || rows_count != m.rows_count) {
        return false;
    }
    for (size_t i = 0; i < m.rows_count; ++i) {
        if (rows_[i] != m.rows_[i]) {
            return false;
        }
    }
    return true;
}

bool Matrix::operator!=(const Matrix& m) const {
    return !(*this == m);
}

std::ostream& operator<<(std::ostream& out, const Matrix& m) {
    for (size_t i = 0; i < m.rows_count; ++i) {
        out << m.rows_[i] << '\n';
    }
    return out;
}

Matrix::ProxyRow::ProxyRow(size_t n) {
    size = n;
    data_ = new int32_t[size];
}

Matrix::ProxyRow::ProxyRow(const Matrix::ProxyRow& r) {
    size = r.size;
    data_ = new int32_t[size];
    if (r.data_ != nullptr) {
        memcpy(data_, r.data_, size * sizeof(*data_));
    }
}

Matrix::ProxyRow::~ProxyRow() {
    if (data_ != nullptr) {
        delete[] data_;
    }
}

Matrix::ProxyRow& Matrix::ProxyRow::operator=(Matrix::ProxyRow r) {
    std::swap(size, r.size);
    std::swap(data_, r.data_);
    return *this;
}

int32_t& Matrix::ProxyRow::operator[](size_t i) {
    if (i >= size) {
        throw std::out_of_range("");
    }
    return data_[i];
}

int32_t Matrix::ProxyRow::operator[](size_t i) const {
    if (i >= size) {
        throw std::out_of_range("");
    }
    return data_[i];
}

Matrix::ProxyRow& Matrix::ProxyRow::operator*=(int32_t x) {
    for (size_t i = 0; i < size; ++i) {
        data_[i] *= x;
    }
    return *this;
}

Matrix::ProxyRow& Matrix::ProxyRow::operator+=(const Matrix::ProxyRow &r) {
    if (size != r.size) {
        throw std::out_of_range("");
    }
    for (size_t i = 0; i < size; ++i) {
        data_[i] += r.data_[i];
    }
    return *this;
}

Matrix::ProxyRow operator+(const Matrix::ProxyRow& r1, const Matrix::ProxyRow& r2) {
    Matrix::ProxyRow tmp(r1);
    tmp += r2;
    return tmp;
}

bool Matrix::ProxyRow::operator==(const Matrix::ProxyRow& r2) const {
    if (size != r2.size) {
        return false;
    }
    for (size_t i = 0; i < size; ++i) {
        if (data_[i] != r2.data_[i]) {
            return false;
        }
    }
    return true;
}

bool Matrix::ProxyRow::operator!=(const Matrix::ProxyRow& r2) const {
    return !(*this == r2);
}

std::ostream& operator<<(std::ostream& out, const Matrix::ProxyRow& r) {
    for (size_t i = 0; i < r.size; ++i) {
        out << r.data_[i] << ' ';
    }
    return out;
}
