#pragma once
#include <utility>
#include "allocator.hpp"
#include "iterator.hpp"

template<typename T, typename Allocator_type = Allocator<T>>
class Vector {
 public:
    using value_type = T;
    using allocator_type = Allocator_type;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using reference = value_type &;
    using const_reference = const value_type &;
    using pointer = value_type *;
    using const_pointer = const value_type *;
    using iterator = Iterator<T>;
    using reverse_iterator = std::reverse_iterator<iterator>;

 private:
    Allocator_type allocator;
    pointer data;
    size_type size_;
    size_type capacity_;

 public:
    Vector() : data(nullptr), size_(0), capacity_(0) {}
    explicit Vector(size_type n);
    Vector(const Vector<T, Allocator_type> &other);
    Vector<T, Allocator_type> &operator=(Vector<T, Allocator_type> other);
    ~Vector();
    reference operator[](int i);
    value_type operator[](int i) const;
    void push_back(const_reference element);
    void pop_back();
    bool empty() const;
    size_type size() const;
    void clear();
    iterator begin();
    iterator end();
    reverse_iterator rbegin();
    reverse_iterator rend();
    void resize(size_type size);
    void reserve(size_type size);
    size_type capacity() const;
    template<typename Type, typename Alloc>
    friend void swap(Vector<Type, Alloc> &first, Vector<Type, Alloc> &second);
    template<typename... Args>
    reference emplace_back(Args&&... element);
};


template<typename T, typename Allocator_type>
Vector<T, Allocator_type>::Vector(size_type n) {
    size_ = n;
    capacity_ = n;
    data = allocator.allocate(capacity_);
    for (size_type i = 0; i < size_; ++i) {
        allocator.construct(data + i);
    }
}

template<typename T, typename Allocator_type>
Vector<T, Allocator_type>::Vector(const Vector<T, Allocator_type>& other) {
    size_ = other.size_;
    capacity_ = other.capacity_;
    data = allocator.allocate(capacity_);
    for (size_type i = 0; i < size_; ++i) {
        allocator.construct(data + i);
        data[i] = other.data[i];
    }
}

template<typename T, typename Allocator_type>
Vector<T, Allocator_type>::~Vector() {
    clear();
    allocator.deallocate(data, capacity_);
}

template<typename T, typename Allocator_type>
Vector<T, Allocator_type>& Vector<T, Allocator_type>::operator=(Vector<T, Allocator_type> other) {
    swap(*this, other);
    return *this;
}

template<typename T, typename Allocator_type>
void swap(Vector<T, Allocator_type>& first, Vector<T, Allocator_type>& second) {
    std::swap(first.allocator, second.allocator);
    std::swap(first.data, second.data);
    std::swap(first.capacity_, second.capacity_);
    std::swap(first.size_, second.size_);
}

template<typename T, typename Allocator_type>
typename Vector<T, Allocator_type>::reference Vector<T, Allocator_type>::operator[](int i) {
    if (i >= 0 && static_cast<size_type>(i) < size_) {
        return data[i];
    }
    throw std::out_of_range("Index out of range");
}

template<typename T, typename Allocator_type>
typename Vector<T, Allocator_type>::value_type Vector<T, Allocator_type>::operator[](int i) const {
    if (i >= 0 && static_cast<Vector<T, Allocator_type>::size_type>(i) < size_) {
        return data[i];
    }
    throw std::out_of_range("Index out of range");
}

template<typename T, typename Allocator_type>
typename Vector<T, Allocator_type>::size_type Vector<T, Allocator_type>::size() const {
    return size_;
}

template<typename T, typename Allocator_type>
typename Vector<T, Allocator_type>::size_type Vector<T, Allocator_type>::capacity() const {
    return capacity_;
}

template<typename T, typename Allocator_type>
bool Vector<T, Allocator_type>::empty() const {
    return size_ == 0;
}

template<typename T, typename Allocator_type>
typename Vector<T, Allocator_type>::iterator Vector<T, Allocator_type>::begin() {
    return iterator(data);
}

template<typename T, typename Allocator_type>
typename Vector<T, Allocator_type>::iterator Vector<T, Allocator_type>::end() {
    return iterator(data + size_);
}

template<typename T, typename Allocator_type>
typename Vector<T, Allocator_type>::reverse_iterator Vector<T, Allocator_type>::rend() {
    return reverse_iterator(iterator(data));
}

template<typename T, typename Allocator_type>
typename Vector<T, Allocator_type>::reverse_iterator Vector<T, Allocator_type>::rbegin() {
    return reverse_iterator(iterator(data + size_));
}

template<typename T, typename Allocator_type>
void Vector<T, Allocator_type>::clear() {
    for (size_type i = 0; i < size_; ++i) {
        allocator.destroy(data + i);
    }
    size_ = 0;
}

template<typename T, typename Allocator_type>
void Vector<T, Allocator_type>::reserve(size_type n) {
    if (n > capacity_) {
        pointer new_data = allocator.allocate(n);
        for (size_type i = 0; i < size_; ++i) {
            allocator.construct(new_data + i, std::forward<value_type>(*(data + i)));
            allocator.destroy(data + i);
        }
        allocator.deallocate(data, capacity_);
        data = new_data;
        capacity_ = n;
    }
}

template<typename T, typename Allocator_type>
void Vector<T, Allocator_type>::resize(size_type n) {
    if (n > size_) {
        reserve(n);
        for (size_type i = size_; i < n; ++i) {
            allocator.construct(data + i);
        }
    } else if (n < size_) {
        pointer new_data = allocator.allocate(n);
        for (size_type i = 0; i < n; ++i) {
            allocator.construct(new_data + i, std::forward<value_type>(*(data + i)));
            allocator.destroy(data + i);
        }
        for (size_type i = n; i < size_; ++i) {
            allocator.destroy(data + i);
        }
        allocator.deallocate(data, capacity_);
        data = new_data;
        capacity_ = n;
    }
    size_ = n;
}

template<typename T, typename Allocator_type>
void Vector<T, Allocator_type>::pop_back() {
    if (size_ > 0) {
        --size_;
        allocator.destroy(data);
        if (2 * size_ < capacity_) {
            pointer new_data = allocator.allocate(2 * size_);
            for (size_type i = 0; i < size_; ++i) {
                allocator.construct(new_data + i, std::forward<value_type>(*(data + i)));
            }
            allocator.deallocate(data, capacity_);
            data = new_data;
            capacity_ = 2 * size_;
        }
    }
}

template<typename T, typename Allocator_type>
void Vector<T, Allocator_type>::push_back(const_reference element) {
    if (size_ >= capacity_) {
        reserve(2 * capacity_ + 1);
    }
    allocator.construct(data + size_, element);
    ++size_;
}


template<typename T, typename Allocator_type>template<typename... Args>
typename Vector<T, Allocator_type>::reference Vector<T, Allocator_type>::emplace_back(Args&&... args) {
    if (size_ >= capacity_) {
        reserve(2 * capacity_ + 1);
    }
    allocator.construct(data + size_, std::forward<Args>(args)...);
    return data[size_++];
}
