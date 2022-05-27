#pragma once
#include <iostream>
#include <limits>
#include <utility>

template<typename T>
class Allocator {
 public:
    using value_type = T;
    using pointer = T*;
    using const_pointer = const T*;
    using reference = T&;
    using const_reference = const T&;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    using propagate_on_container_move_assignment = std::true_type;
    using is_always_equal = std::true_type;
    Allocator() = default;
    ~Allocator() = default;

    const_pointer address(const_reference el) const noexcept {
        return &el;
    }

    pointer allocate(size_type size) {
        void* memory = operator new(size * sizeof(T));
        return static_cast<pointer>(memory);
    }

    void deallocate(pointer p, size_type size) {
        operator delete(static_cast<void*>(p));
    }

    size_type max_size() const noexcept {
        return std::numeric_limits<size_type>::max() / sizeof(value_type);
    }

    template<typename... Args>
    void construct(pointer p, Args&&... a) {
        new(p) value_type(std::forward<Args>(a)...);
    }

    void destroy(pointer p) {
        p->~value_type();
    }
};

template<typename T1, typename T2>
constexpr bool operator==(const Allocator<T1>& t1, const Allocator<T2>& t2) noexcept {
    return std::is_same<T1, T2>::value;
}
