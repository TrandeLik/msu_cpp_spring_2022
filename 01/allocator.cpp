#include <iostream>
#include <string>
#include "allocator.h"

int main() {
    std::string inp;
    Allocator a;
    while (std::cin >> inp) {
        if (inp == "make") {
            std::cout << "Please, input memory size" << std::endl;
            size_t x;
            std::cin >> x;
            a.makeAllocator(x);
        }
        if (inp == "alloc") {
            std::cout << "Please, input memory size" << std::endl;
            size_t x;
            std::cin >> x;
            char* res = a.alloc(x);
            if (res == nullptr) {
                std::cout << "There is no available memory" << std::endl;
            } else {
                std::cout << "Memory successfully has been allocated at " <<
                    std::hex << static_cast<void*>(res) << std::endl;
            }
        }
        if (inp == "reset") {
            a.reset();
        }
    }
}

