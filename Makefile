# Makefile for C Collections Project

CC = gcc
CFLAGS = -std=c2x -Wall -Wextra -Ilist
LDFLAGS =

# Directories
LIST_DIR = list
TEST_DIR = test

# Source files
LIST_SRC = $(LIST_DIR)/list.c
TEST_SRC = $(TEST_DIR)/main.c $(TEST_DIR)/test_list.c

# Object files
LIST_OBJ = $(LIST_DIR)/list.o
TEST_OBJ = $(TEST_DIR)/main.o $(TEST_DIR)/test_list.o

# Executables
TEST_EXE = unittest

# Default target
all: $(TEST_EXE)

# Build the test executable
$(TEST_EXE): $(LIST_OBJ) $(TEST_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile list object
$(LIST_DIR)/list.o: $(LIST_DIR)/list.c $(LIST_DIR)/list.h
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test objects
$(TEST_DIR)/main.o: $(TEST_DIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TEST_DIR)/test_list.o: $(TEST_DIR)/test_list.c $(LIST_DIR)/list.h
	$(CC) $(CFLAGS) -c $< -o $@

# Run tests
test: $(TEST_EXE)
	./$(TEST_EXE)

# Clean build artifacts
clean:
	rm -f $(LIST_OBJ) $(TEST_OBJ) $(TEST_EXE)

# Phony targets
.PHONY: all test clean