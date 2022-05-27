#pragma once
#include <iterator>

template<typename T>
class Iterator: public std::iterator<std::random_access_iterator_tag, T> {
 private:
    T* iter;

 public:
    explicit Iterator(T* p): iter(p) {}
    Iterator() = default;
    ~Iterator() = default;
    T& operator*() {
        return *iter;
    }
    const T& operator*() const {
        return *iter;
    }
    Iterator& operator++() {
        ++iter;
        return *this;
    }
    Iterator operator++(int) {
        T* p = iter;
        ++iter;
        return Iterator<T>(p);
    }
    Iterator& operator--() {
        --iter;
        return *this;
    }
    Iterator operator--(int) {
        T* p = iter;
        --iter;
        return Iterator<T>(p);
    }
    Iterator& operator+=(std::size_t n) {
        iter += n;
        return *this;
    }
    Iterator operator+(std::size_t n) {
        Iterator<T> p(iter);
        p += n;
        return p;
    }
    Iterator& operator-=(std::size_t n) {
        iter -= n;
        return *this;
    }
    Iterator operator-(std::size_t n) {
        Iterator<T> p(iter);
        p -= n;
        return p;
    }
    size_t operator-(const Iterator<T>& iterator) {
        if (iter < iterator.iter) {
            throw std::invalid_argument("Can't subtract following iterator from previous");
        }
        return static_cast<size_t>(iter - iterator.iter);
    }
    T& operator[](size_t n) {
        return *(iter + n);
    }
    const T& operator[](size_t n) const {
        return *(iter + n);
    }
    template<typename Type>
    friend bool operator==(const Iterator<Type>& first, const Iterator<Type>& second);
    template<typename Type>
    friend bool operator<(const Iterator<Type>& first, const Iterator<Type>& second);
};

template<typename T>
bool operator==(const Iterator<T>& first, const Iterator<T>& second) {
    return first.iter == second.iter;
}

template<typename T>
bool operator!=(const Iterator<T>& first, const Iterator<T>& second) {
    return !(first == second);
}

template<typename T>
bool operator<(const Iterator<T>& first, const Iterator<T>& second) {
    return first.iter < second.iter;
}

template<typename T>
bool operator>(const Iterator<T>& first, const Iterator<T>& second) {
    return second < first;
}

template<typename T>
bool operator<=(const Iterator<T>& first, const Iterator<T>& second) {
    return !(first > second);
}

template<typename T>
bool operator>=(const Iterator<T>& first, const Iterator<T>& second) {
    return !(first < second);
}
