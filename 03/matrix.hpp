#pragma once
#include <cstdint>
#include <iostream>
#include <cstring>

class Matrix {
    class ProxyRow {
     private:
        size_t size;
        int32_t* data_;
     public:
        ProxyRow(): size(0), data_(nullptr) {}
        explicit ProxyRow(size_t n);
        ProxyRow(const ProxyRow& r);
        int32_t& operator[](size_t i);
        int32_t operator[](size_t i) const;
        ProxyRow& operator=(ProxyRow r);
        ProxyRow& operator*=(int32_t x);
        ProxyRow& operator+=(const ProxyRow& r);
        bool operator==(const ProxyRow& r2) const;
        bool operator!=(const ProxyRow& r2) const;
        friend std::ostream& operator<<(std::ostream& out, const ProxyRow& r);
        ~ProxyRow();
    };
    size_t rows_count;
    size_t cols_count;
    ProxyRow* rows_;

 public:
    Matrix(): rows_count(0), cols_count(0), rows_(nullptr) {}
    Matrix(size_t n, size_t m);
    explicit Matrix(size_t n);
    Matrix(const Matrix& m);
    size_t get_rows() const;
    size_t get_cols() const;
    ProxyRow& operator[](size_t i);
    ProxyRow operator[](size_t i) const;
    Matrix& operator=(Matrix m);
    Matrix& operator*=(int32_t x);
    Matrix& operator+=(const Matrix& m);
    bool operator==(const Matrix& m) const;
    bool operator!=(const Matrix& m) const;
    friend std::ostream& operator<<(std::ostream& out, const Matrix& m);
    friend std::ostream& operator<<(std::ostream& out, const ProxyRow& r);
    friend Matrix::ProxyRow operator+(const Matrix::ProxyRow& r1, const Matrix::ProxyRow& r2);
    ~Matrix();
};

Matrix operator+(const Matrix& m1, const Matrix& m2);
