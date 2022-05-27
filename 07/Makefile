CC=g++
FLAGS=-std=c++2a -Wall -fsanitize=undefined,address
TESTING_FLAGS=-lgtest_main -lgtest -lpthread

all:
		$(CC) $(FLAGS) -c vector.hpp -o vector-lib
test: test_file
		./test_file
test_file:
		$(CC) $(FLAGS) $(TESTING_FLAGS) test_vector.cpp -o test_file
clean:
		rm -f vector-lib test_file