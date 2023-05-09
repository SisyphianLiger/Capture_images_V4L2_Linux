CC = gcc
CFLAGS = -Wall -I headers
SRC_DIR = src
BIN_DIR = bin
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Default target
all: $(BIN_DIR)/main 

# Targets
$(BIN_DIR)/main: main.c $(SRCS)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

# Clean
clean:
	rm -rf $(BIN_DIR)
