CC=g++
FLAGS=-std=c++2a -Wall -fsanitize=undefined,address
TESTING_FLAGS=-lgtest_main -lgtest -lpthread

all:
		$(CC) $(FLAGS) -c format.hpp -o format-lib
test: test_file
		./test_file
test_file:
		$(CC) $(FLAGS) $(TESTING_FLAGS) test_format.cpp -o test_file
clean:
		rm -f format-lib test_file