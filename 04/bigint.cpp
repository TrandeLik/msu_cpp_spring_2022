#include <algorithm>
#include "bigint.hpp"

void BigInt::push_back(int32_t x) {
    if (size == real_size) {
        real_size = 2 * real_size + 1;
        auto new_mem = new int32_t[real_size];
        for (size_t i = 0; i < size; ++i) {
            new_mem[i] = bits[i];
        }
        delete[] bits;
        bits = new_mem;
    }
    bits[size++] = x;
}

void BigInt::resize(size_t new_size) {
    int32_t* new_mem = nullptr;
    if (new_size != 0) {
        new_mem = new int32_t[new_size];
        for (size_t i = 0; i < new_size; ++i) {
            new_mem[i] = 0;
        }
        for (size_t i = 0; i < std::min(size, new_size); ++i) {
            new_mem[i] = bits[i];
        }
    }
    delete[] bits;
    bits = new_mem;
    real_size = new_size;
    size = new_size;
}

void BigInt::clear() {
    delete[] bits;
    bits = nullptr;
    real_size = 0;
    size = 0;
}

BigInt::BigInt(): bits(nullptr), size(0), real_size(0) {
    push_back(0);
}

BigInt::BigInt(const BigInt& num): bits(nullptr), size(0), real_size(0) {
    is_positive = num.is_positive;
    delete[] bits;
    size = num.size;
    real_size = num.real_size;
    bits = new int32_t[real_size];
    for (size_t i = 0; i < size; ++i) {
        bits[i] = num.bits[i];
    }
}

BigInt::BigInt(const std::string& s): bits(nullptr), size(0), real_size(0) {
    parse_string(s);
}

BigInt::BigInt(BigInt&& other): bits(nullptr), size(0), real_size(0) {
    bits = other.bits;
    size = other.size;
    real_size = other.real_size;
    is_positive = other.is_positive;
    other.bits = nullptr;
    other.size = 0;
    other.real_size = 0;
    other.is_positive = true;
    other.push_back(0);
}

BigInt::BigInt(int32_t value): bits(nullptr), size(0), real_size(0) {
    if (value == 0) {
        push_back(0);
        is_positive = true;
        return;
    }
    int32_t num = value;
    if (num < 0) {
        is_positive = false;
        num *= -1;
    }
    while (num) {
        push_back(num % base);
        num /= base;
    }
}

BigInt::~BigInt() {
    delete[] bits;
}

void BigInt::swap(BigInt& num) {
    std::swap(bits, num.bits);
    std::swap(is_positive, num.is_positive);
    std::swap(size, num.size);
    std::swap(real_size, num.real_size);
}

BigInt& BigInt::operator=(const BigInt& num) {
    BigInt tmp(num);
    swap(tmp);
    return *this;
}

BigInt& BigInt::operator=(BigInt&& other) {
    if (this != &other) {
        clear();
    }
    bits = other.bits;
    size = other.size;
    real_size = other.real_size;
    is_positive = other.is_positive;
    other.bits = nullptr;
    other.size = 0;
    other.real_size = 0;
    other.is_positive = true;
    other.push_back(0);
    return *this;
}

BigInt& BigInt::operator=(int value) {
    size = 0;
    real_size = 0;
    is_positive = true;
    if (value == 0) {
        push_back(0);
        return *this;
    }
    int32_t num = value;
    if (num < 0) {
        is_positive = false;
        num *= -1;
    }
    while (num) {
        push_back(num % base);
        num /= base;
    }
    return *this;
}

bool operator==(const BigInt& num1, const BigInt& num2) {
    if (num1.size != num2.size || num1.is_positive != num2.is_positive) {
        return false;
    }
    for (size_t i = 0; i < num1.size; ++i) {
        if (num1.bits[i] != num2.bits[i]) {
            return false;
        }
    }
    return true;
}

bool operator!=(const BigInt& num1, const BigInt& num2) {
    return !(num1 == num2);
}

bool operator<(const BigInt& num1, const BigInt& num2) {
    if (!num1.is_positive && num2.is_positive) {
        return true;
    }
    if (num1.is_positive && !num2.is_positive) {
        return false;
    }
    if (num1.size < num2.size) {
        return num1.is_positive;
    }
    if (num1.size > num2.size) {
        return !num1.is_positive;
    }
    for (int32_t i = static_cast<int32_t>(num1.size - 1); i >= 0; --i) {
        if (num1.bits[i] != num2.bits[i]) {
            return (num1.bits[i] < num2.bits[i]) == num1.is_positive;
        }
    }
    return false;
}

bool operator<=(const BigInt& num1, const BigInt& num2) {
    return (num1 < num2) || (num1 == num2);
}

bool operator>(const BigInt& num1, const BigInt& num2) {
    return !(num1 <= num2);
}

bool operator>=(const BigInt& num1, const BigInt& num2) {
    return !(num1 < num2);
}

bool BigInt::less_abs(const BigInt& num) const {
    if (size < num.size) {
        return true;
    }
    if (size > num.size) {
        return false;
    }
    for (int32_t i = static_cast<int32_t>(size - 1); i >= 0; --i)
        if (bits[i] != num.bits[i]) {
            return (bits[i] < num.bits[i]);
        }
    return false;
}

std::ostream& operator<<(std::ostream& out, const BigInt& num) {
    out << num.toString();
    return out;
}

BigInt& BigInt::change_sign() {
    if (bits[size - 1] == 0) {
        return *this;
    }
    is_positive = (!is_positive);
    return *this;
}

void BigInt::remove_extra_zeros() {
    size_t zeros = 0;
    for (int32_t i = static_cast<int32_t>(size - 1); i >= 0; --i) {
        if (bits[i] == 0) {
            ++zeros;
        } else {
            break;
        }
    }
    if (zeros) {
        resize(size - zeros);
    }
    if (size == 0) {
        push_back(0);
    }
}

void BigInt::shift_right() {
    if (size == 0) {
        push_back(0);
        return;
    }
    push_back(bits[size - 1]);
    for (int32_t i = static_cast<int32_t>(size - 2); i > 0; --i) {
        bits[i] = bits[i - 1];
    }
    bits[0] = 0;
}

BigInt& BigInt::operator+=(const BigInt& num) {
    if (*this == 0) {
        *this = num;
        return *this;
    }
    if (num == 0) {
        return *this;
    }
    if ((!is_positive && num.is_positive) || (is_positive && !num.is_positive)) {
        change_sign();
        *this -= num;
        change_sign();
        return *this;
    }
    int32_t carry = 0;
    for (size_t i = 0; i < std::max(size, num.size) || carry; ++i) {
        if (i == size) {
            push_back(0);
        }
        bits[i] += carry + (i < num.size ? num.bits[i] : 0);
        carry = bits[i] >= BigInt::base;
        if (carry) {
            bits[i] -= BigInt::base;
        }
    }
    return *this;
}

BigInt operator+(const BigInt& num1, const BigInt& num2) {
    BigInt copy = num1;
    copy += num2;
    return copy;
}

void BigInt::subtraction(const BigInt& bigger, const BigInt& smaller) {
    int32_t carry = 0;
    resize(bigger.size);
    for (size_t i = 0; i < smaller.size || carry; ++i) {
        bits[i] = bigger.bits[i] - carry - (i < smaller.size ? smaller.bits[i] : 0);
        carry = bits[i] < 0;
        if (carry) {
            bits[i] += BigInt::base;
        }
    }
    remove_extra_zeros();
}

BigInt& BigInt::operator-=(const BigInt& num) {
    if (*this == 0) {
        *this = num;
        change_sign();
        return *this;
    }
    if (num == 0) {
        return *this;
    }
    if (*this == num) {
        clear();
        push_back(0);
        is_positive = true;
        return *this;
    }
    if ((!is_positive && num.is_positive) || (is_positive && !num.is_positive)) {
        change_sign();
        *this += num;
        change_sign();
        return *this;
    }
    if (less_abs(num)) {
        subtraction(num, *this);
        change_sign();
        return *this;
    }
    subtraction(*this, num);
    return *this;
}

BigInt operator-(const BigInt& num1, const BigInt& num2) {
    BigInt copy = num1;
    copy -= num2;
    return copy;
}

BigInt& BigInt::operator*=(const BigInt& num) {
    if (num == 0 || *this == 0) {
        *this = 0;
        return *this;
    }
    BigInt result;
    result.is_positive = is_positive == num.is_positive;
    result.resize(size + num.size);
    for (size_t i = 0; i < size; ++i) {
        int32_t carry = 0;
        for (size_t j = 0; j < num.size || carry; ++j) {
            int64_t cur = result.bits[i + j] + static_cast<int64_t>(bits[i]) * (j < num.size ? num.bits[j] : 0) + carry;
            result.bits[i + j] = static_cast<int32_t>(cur % BigInt::base);
            carry = static_cast<int32_t>(cur / BigInt::base);
        }
    }
    result.remove_extra_zeros();
    swap(result);
    return *this;
}

BigInt operator*(const BigInt& num1, const BigInt& num2) {
    BigInt copy = num1;
    copy *= num2;
    return copy;
}

BigInt BigInt::operator-() const {
    BigInt copy = *this;
    if (copy == 0) {
        return copy;
    }
    copy.is_positive = !(copy.is_positive);
    return copy;
}

BigInt& BigInt::parse_string(const std::string& s) {
    clear();
    is_positive = true;
    int32_t last = 0;
    if (s[0] == '-') {
        is_positive = false;
        last = 1;
    }
    int32_t s_len = s.size();
    for (int32_t i = s_len - 1; i >= last; i -= 4) {
        size_t len = (i >= 4) ? 4 : i - last + 1;
        push_back(stoi(s.substr(i - len + 1, len)));
    }
    return *this;
}

std::string BigInt::toString() const {
    std::string s;
    if (!is_positive) {
        s += "-";
    }
    for (int32_t i = size - 1; i >= 0; --i) {
        if (i != static_cast<int32_t>(size - 1) && bits[i] == 0) {
            s += "0000";
        } else if (i == static_cast<int32_t>(size - 1)) {
            s += std::to_string(bits[i]);
        } else {
            if (bits[i] > 999) {
                s += std::to_string(bits[i]);
            } else if (bits[i] > 99) {
                s += "0" + std::to_string(bits[i]);
            } else if (bits[i] > 9) {
                s += "00" + std::to_string(bits[i]);
            } else if (bits[i] > 0) {
                s += "000" + std::to_string(bits[i]);
            }
        }
    }
    return s;
}

