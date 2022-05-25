#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <utility>

class BigInt {
 private:
    static const int32_t base = 1e4;
    int32_t* bits;
    size_t size;
    size_t real_size;
    bool is_positive = true;
    void swap(BigInt&);
    void remove_extra_zeros();
    void shift_right();
    void push_back(int32_t);
    void resize(size_t);
    void clear();
    bool less_abs(const BigInt&) const;

 public:
    BigInt();
    BigInt(const BigInt&);
    BigInt(const std::string&);
    BigInt(BigInt&&);
    BigInt(int32_t);
    ~BigInt();
    BigInt& operator=(const BigInt&);
    BigInt& operator=(BigInt&&);
    BigInt& operator=(int32_t);
    BigInt& change_sign();
    friend bool operator==(const BigInt&, const BigInt&);
    friend bool operator<(const BigInt&, const BigInt&);
    BigInt& parse_string(const std::string& s);
    std::string toString() const;
    friend std::ostream& operator<<(std::ostream&, const BigInt&);
    BigInt& operator+=(const BigInt&);
    BigInt& operator-=(const BigInt&);
    BigInt& operator*=(const BigInt&);
    void subtraction(const BigInt&, const BigInt&);
    BigInt operator-() const;
};

BigInt operator+(const BigInt& num1, const BigInt& num2);
BigInt operator-(const BigInt& num1, const BigInt& num2);
BigInt operator*(const BigInt& num1, const BigInt& num2);

bool operator!=(const BigInt& num1, const BigInt& num2);
bool operator<=(const BigInt& num1, const BigInt& num2);
bool operator>=(const BigInt& num1, const BigInt& num2);
bool operator>(const BigInt& num1, const BigInt& num2);
