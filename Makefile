CC = gcc
CFLAGS = -Wall -Iheaders
SRC_DIR = src
BIN_DIR = bin
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(SRCS))

all: $(BIN_DIR)/main

$(BIN_DIR)/main: $(OBJS) main.c
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(BIN_DIR)/main
	./$(BIN_DIR)/main

clean:
	rm -f $(BIN_DIR)/main $(BIN_DIR)/*.o

