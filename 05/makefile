CC=g++
FLAGS=-std=c++2a -Wall -Werror -fsanitize=undefined,address
TESTING_FLAGS=-lgtest_main -lgtest -lpthread

all:
		$(CC) $(FLAGS) -c serialize_deserialize_simple_types.cpp -o serializer-lib
test: test_file
		./test_file
test_file:
		$(CC) $(FLAGS) $(TESTING_FLAGS) test_serializer.cpp serialize_deserialize_simple_types.cpp -o test_file
clean:
		rm -f serializer-lib test_file