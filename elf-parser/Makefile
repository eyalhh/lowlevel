CC := cc
CFLAGS := -std=c11 -Wall -Wextra -g -Iinclude
SRC := $(wildcard src/*.c)
OBJ := $(SRC:src/%.c=build/%.o)
HDR := $(wildcard include/elfparser/*.h)
BIN := elfparser

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) -o $@ -static -no-pie $(OBJ)

build/%.o: src/%.c $(HDR) | build
	$(CC) $(CFLAGS) -c $< -o $@

build:
	mkdir -p build

clean:
	rm -rf build

.PHONY: all clean
