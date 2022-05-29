CC=g++
FLAGS=-std=c++2a -Wall -fsanitize=undefined,address
THREADING=-pthread
TESTING_FLAGS=-lgtest_main -lgtest -lpthread

all:
		$(CC) $(FLAGS) $(THREADING) -c sort.cpp -o sort-lib
test: test_file
		./test_file
test_file:
		$(CC) $(FLAGS) $(THREADING) $(TESTING_FLAGS) test_sort.cpp sort.cpp -o test_file
clean:
		rm -f sort-lib test_file