CC=g++
FLAGS=-std=c++2a -Wall -Werror -fsanitize=undefined,address
TESTING_FLAGS=-lgtest_main -lgtest -lpthread

all: token_parser.cpp
		$(CC) $(FLAGS) token_parser.cpp -o parser
test: test_file
		./test_file
test_file: test_parser.cpp
		$(CC) $(FLAGS) $(TESTING_FLAGS) test_parser.cpp -o test_file
clean:
		rm -f parser test_file
run: all
	./parser