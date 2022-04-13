CC=g++
FLAGS=-std=c++2a -Wall -Werror -fsanitize=undefined,address
TESTING_FLAGS=-lgtest_main -lgtest -lpthread

all:
		$(CC) $(FLAGS) -c matrix.cpp -o matrix-lib
test: test_file
		./test_file
test_file:
		$(CC) $(FLAGS) $(TESTING_FLAGS) matrix.cpp test_matrix.cpp -o test_file
clean:
		rm -f matrix-lib test_file