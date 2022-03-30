CC=g++
FLAGS=-std=c++2a -Wall -Werror -fsanitize=undefined,address
TESTING_FLAGS=-lgtest_main -lgtest -lpthread

all: allocator.cpp
		$(CC) $(FLAGS) allocator.cpp -o allocator
test: test_file
		./test_file
test_file: test_allocator.cpp
		$(CC) $(FLAGS) $(TESTING_FLAGS) test_allocator.cpp -o test_file
clean:
		rm -f allocator test_file