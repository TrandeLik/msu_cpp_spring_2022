CC=g++
FLAGS=-std=c++2a -Wall -Werror -fsanitize=undefined,address
TESTING_FLAGS=-lgtest_main -lgtest -lpthread

all:
		$(CC) $(FLAGS) -c bigint.cpp -o bigint-lib
test: test_file
		./test_file
test_file:
		$(CC) $(FLAGS) $(TESTING_FLAGS) bigint.cpp test_bigint.cpp -o test_file
clean:
		rm -f bigint-lib test_file