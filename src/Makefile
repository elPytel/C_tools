CC = gcc
CFLAGS = -Wall -Wextra -I.
EXT = .bin
TARGET = test_message$(EXT)
SRCS = test_message.c message.c
TEST_SRCS = test_message.c test_data_block.c message.c
TEST_TARGETS = test_message$(EXT) test_data_block$(EXT)

all: $(TARGET) $(TEST_TARGETS)

$(TARGET): $(SRCS)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

test_message$(EXT): test_message.c message.c
	$(CC) $(CFLAGS) -o test_message$(EXT) test_message.c message.c

test_data_block$(EXT): test_data_block.c message.c
	$(CC) $(CFLAGS) -o test_data_block$(EXT) test_data_block.c message.c

clean:
	rm -f $(TARGET) $(TEST_TARGETS)

test: $(TARGET)
	./$(TARGET)

test_all: $(TEST_TARGETS)
	./test_message$(EXT)
	./test_data_block$(EXT)

valgrind: $(TARGET)
	valgrind --leak-check=full ./$(TARGET)

valgrind_all: $(TEST_TARGETS)
	valgrind --leak-check=full ./test_message$(EXT)
	valgrind --leak-check=full ./test_data_block$(EXT)

.PHONY: all clean test test_all valgrind valgrind_all