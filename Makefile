# Makefile for lab1

CC	:= gcc
CFLAGS	:= -std=c11 -Wall -Wextra -Wpedantic -O2 -g
LDFLAGS :=

SRC_DIR	:= src
BIN_DIR	:= .

PROGS	:= fork_identity proc_tree fork_exec memory_perf

all: $(PROGS)

fork_identity: $(SRC_DIR)/fork_identity.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $< $(LDFLAGS)

proc_tree: $(SRC_DIR)/proc_tree.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $< $(LDFLAGS)

fork_exec: $(SRC_DIR)/fork_exec.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $< $(LDFLAGS)

memory_perf: $(SRC_DIR)/memory_perf.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $< $(LDFLAGS) -lrt

clean:
	rm -f $(PROGS)
