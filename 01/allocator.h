#include <cstddef>

class Allocator {
 private:
    char* memory;
    size_t offset;
    size_t max_size;

 public:
    Allocator(): memory(nullptr), offset(0), max_size(0) {}
    Allocator(const Allocator&) = delete;
    Allocator& operator=(const Allocator&) = delete;

    void makeAllocator(size_t maximal_size) {
        if (memory != nullptr) {
            delete[] memory;
        }
        max_size = maximal_size;
        memory = new char[max_size];
    }

    char* alloc(size_t size) {
        if (size > 0 && offset + size < max_size) {
            offset += size;
            return memory + offset - size;
        }
        return nullptr;
    }

    void reset() {
        offset = 0;
    }

    ~Allocator() {
        delete[] memory;
    }
};
