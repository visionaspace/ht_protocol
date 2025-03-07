# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Iinc
LDFLAGS =

# Directories
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = build/
EXEC = ht_example

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

build: $(OBJ_DIR) $(OBJS)
	$(CC) main.c $(OBJS) $(CFLAGS) -o $(EXEC)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Ensure obj directory exists
$(OBJ_DIR):
	mkdir -p $@

run:
	./$(EXEC)

# Clean build files
.PHONY: clean build run distclean

clean: distclean
	rm -rf $(OBJS) $(EXEC)

distclean:
	rm -rf $(OBJ_DIR)
