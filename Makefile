# Compiler and flags
CC = gcc
AR = ar
CFLAGS = -Wall -Wextra -Iinc
LDFLAGS =

# Cross-compiling
CROSS_PREFIX ?=
CROSS_FLAGS ?=

# Directories
SRC_DIR = src
INC_DIR = inc
OBJ_DIR = build/

# Generated files
LIB = libht.a
EXEC = ht_example

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.c)

OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

build: $(OBJ_DIR) $(OBJS)
	$(CROSS_PREFIX)$(CC) main.c $(OBJS) $(CFLAGS) $(CROSS_FLAGS) -o $(EXEC)

$(LIB): $(OBJ_DIR) $(OBJ_DIR)/ht_protocol.o
	$(CROSS_PREFIX)$(AR) rcs $@ $(OBJ_DIR)/ht_protocol.o

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CROSS_PREFIX)$(CC) $(CFLAGS) $(CROSS_FLAGS) -c $< -o $@

# Ensure obj directory exists
$(OBJ_DIR):
	mkdir -p $@

run:
	./$(EXEC)

# Clean build files
.PHONY: clean build run distclean

clean: distclean
	rm -rf $(OBJS) $(EXEC) $(LIB)

distclean:
	rm -rf $(OBJ_DIR)
