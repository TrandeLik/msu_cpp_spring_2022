CC=g++
FLAGS=-std=c++2a -Wall -fsanitize=undefined,address
THREADING=-pthread
TESTING_FLAGS=-lgtest_main -lgtest -lpthread

all:
		$(CC) $(FLAGS) $(THREADING) -c thread_pool.cpp -o thread_pool-lib
test: test_file
		./test_file
test_file:
		$(CC) $(FLAGS) $(THREADING) $(TESTING_FLAGS) test_thread_pool.cpp thread_pool.cpp -o test_file
clean:
		rm -f thread_pool-lib test_file