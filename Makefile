# Makefile for C Collections Project

CC = gcc
CFLAGS = -std=c2x -Wall -Wextra -Iinclude
LDFLAGS =

# Directories
SRC_DIR = src
INC_DIR = include
TEST_DIR = test
BUILD_DIR = build
LIST_SRC_DIR = $(SRC_DIR)/list
TEST_SRC_DIR = $(TEST_DIR)/list

# Source files
LIST_SRC = $(LIST_SRC_DIR)/list.c $(LIST_SRC_DIR)/doubly_linked_list.c
TEST_SRC = $(TEST_DIR)/main.c $(TEST_SRC_DIR)/test_list.c $(TEST_SRC_DIR)/test_doubly_linked_list.c

# Object files
LIST_OBJ = $(BUILD_DIR)/list.o $(BUILD_DIR)/doubly_linked_list.o
TEST_OBJ = $(BUILD_DIR)/main.o $(BUILD_DIR)/test_list.o $(BUILD_DIR)/test_doubly_linked_list.o

# Executables
TEST_EXE = $(BUILD_DIR)/unittest

# Default target
all: $(TEST_EXE)

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build the test executable
$(TEST_EXE): $(LIST_OBJ) $(TEST_OBJ) | $(BUILD_DIR)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile list object
$(BUILD_DIR)/list.o: $(LIST_SRC_DIR)/list.c $(INC_DIR)/list.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile doubly linked list object
$(BUILD_DIR)/doubly_linked_list.o: $(LIST_SRC_DIR)/doubly_linked_list.c $(INC_DIR)/doubly_linked_list.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compile test objects
$(BUILD_DIR)/main.o: $(TEST_DIR)/main.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test_list.o: $(TEST_SRC_DIR)/test_list.c $(INC_DIR)/list.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/test_doubly_linked_list.o: $(TEST_SRC_DIR)/test_doubly_linked_list.c $(INC_DIR)/doubly_linked_list.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Run tests
test: $(TEST_EXE)
	./$(TEST_EXE)

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all test clean