#include <gtest/gtest.h>
#include <fstream>
#include <thread>

auto get_file_size(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary | std::ios::in);
    auto fsize = file.tellg();
    file.seekg(0, std::ios::end);
    fsize = file.tellg() - fsize;
    return fsize;
}

void merge_files(const std::string& filename, const std::string& first_name,
                 const std::string& second_name, std::size_t len_first,
                 std::size_t len_second, std::size_t iter) {
    std::ofstream input(filename, std::ios::binary | std::ios::out);
    std::ifstream first(first_name, std::ios::binary | std::ios::in);
    std::ifstream second(second_name, std::ios::binary | std::ios::in);
    uint64_t first_num, second_num;
    std::size_t first_count = 0;
    std::size_t second_count = 0;
    if (len_first != 0) {
        first.read(reinterpret_cast<char*>(&first_num), sizeof(first_num));
        first_count++;
    }
    if (len_second != 0) {
        second.read(reinterpret_cast<char*>(&second_num), sizeof(second_num));
        second_count++;
    }
    while (first_count <= len_first && second_count <= len_second) {
        std::size_t first_index = 0;
        std::size_t second_index = 0;
        while (first_index < iter && second_index < iter && first_count <= len_first && second_count <= len_second) {
            if (first_num < second_num) {
                input.write(reinterpret_cast<char*>(&first_num), sizeof(first_num));
                first.read(reinterpret_cast<char*>(&first_num), sizeof(first_num));
                first_index++;
                first_count++;
            } else {
                input.write(reinterpret_cast<char*>(&second_num), sizeof(second_num));
                second.read(reinterpret_cast<char*>(&second_num), sizeof(second_num));
                second_index++;
                second_count++;
            }
        }
        while (first_index < iter && first_count <= len_first) {
            input.write(reinterpret_cast<char*>(&first_num), sizeof(first_num));
            first.read(reinterpret_cast<char*>(&first_num), sizeof(first_num));
            first_index++;
            first_count++;
        }
        while (second_index < iter && second_count <= len_second) {
            input.write(reinterpret_cast<char*>(&second_num), sizeof(second_num));
            second.read(reinterpret_cast<char*>(&second_num), sizeof(second_num));
            second_index++;
            second_count++;
        }
    }
    while (first_count <= len_first) {
        input.write(reinterpret_cast<char*>(&first_num), sizeof(first_num));
        first.read(reinterpret_cast<char*>(&first_num), sizeof(first_num));
        first_count++;
    }
    while (second_count <= len_second) {
        input.write(reinterpret_cast<char*>(&second_num), sizeof(second_num));
        second.read(reinterpret_cast<char*>(&second_num), sizeof(second_num));
        second_count++;
    }
    input.close();
    first.close();
    second.close();
}

void sort_file(const std::string& filename, std::size_t len, std::size_t pid) {
    uint64_t first_num, second_num;
    std::size_t iter = 1;
    std::stringstream prefix;
    prefix << pid;
    std::string first_name = prefix.str() + "for_sort_first.bin";
    std::string second_name = prefix.str() + "for_sort_second.bin";
    while (iter < len) {
        std::size_t len_first = 0, len_second = 0;
        std::ifstream input(filename, std::ios::binary | std::ios::in);
        std::ofstream first(first_name, std::ios::binary | std::ios::out);
        std::ofstream second(second_name, std::ios::binary | std::ios::out);
        std::size_t j = 0;
        while (j < len) {
            for (std::size_t i = 0; j < len && i < iter; ++i) {
                input.read(reinterpret_cast<char*>(&first_num), sizeof(first_num));
                first.write(reinterpret_cast<char*>(&first_num), sizeof(first_num));
                len_first++;
                j++;
            }
            for (std::size_t i = 0; j < len && i < iter; ++i) {
                input.read(reinterpret_cast<char*>(&second_num), sizeof(second_num));
                second.write(reinterpret_cast<char*>(&second_num), sizeof(second_num));
                len_second++;
                j++;
            }
        }
        input.close();
        first.close();
        second.close();
        merge_files(filename, first_name, second_name, len_first, len_second, iter);
        iter *= 2;
    }
    std::remove(first_name.c_str());
    std::remove(second_name.c_str());
}

std::string split_and_sort(const std::string& filename, std::size_t len, std::size_t pid) {
    std::ifstream input(filename, std::ios::binary | std::ios::in);
    std::size_t start_pos = 0;
    std::size_t end_pos = len / 2;
    if (pid) {
        start_pos = len / 2;
        end_pos = len;
    }
    input.seekg(static_cast<long>(start_pos * sizeof(uint64_t)), std::ios::beg);
    std::stringstream prefix;
    prefix << "pid-"<< pid;
    std::string out_name = prefix.str() + filename;
    std::ofstream output(out_name, std::ios::binary | std::ios::out);
    uint64_t el;
    for (std::size_t i = start_pos; i < end_pos; ++i) {
        input.read(reinterpret_cast<char*>(&el), sizeof(el));
        output.write(reinterpret_cast<char*>(&el), sizeof(el));
    }
    input.close();
    output.close();
    sort_file(out_name, end_pos - start_pos, pid);
    return out_name;
}

void sort(const std::string& filename) {
    auto filesize = get_file_size(filename) / sizeof(uint64_t);
    if (filesize > 1) {
        std::string first_name, second_name;
        std::thread thread1([&first_name, filename, filesize]() {
            first_name = split_and_sort(filename, filesize, 0);
        });
        std::thread thread2([&second_name, filename, filesize]() {
            second_name = split_and_sort(filename, filesize, 1);
        });
        thread1.join();
        thread2.join();
        auto first_size = get_file_size(first_name) / sizeof(uint64_t);
        auto second_size = get_file_size(second_name) / sizeof(uint64_t);
        merge_files("sorted_" + filename, first_name, second_name,
                    first_size, second_size, std::max(first_size, second_size));
        std::remove(first_name.c_str());
        std::remove(second_name.c_str());
    } else if (filesize == 1) {
        std::ifstream input(filename, std::ios::binary | std::ios::in);
        std::ofstream output("sorted_" + filename, std::ios::binary | std::ios::out);
        uint64_t el;
        input.read(reinterpret_cast<char*>(&el), sizeof(el));
        output.write(reinterpret_cast<char*>(&el), sizeof(el));
    } else {
        throw std::invalid_argument("Invalid file size\n");
    }
}