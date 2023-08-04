CC = gcc
CFLAGS := -Wall -Wextra -pedantic

SRC = src/*.c
OUT = bin/out.exe

all: build run
build: $(SRC)
	$(CC) $(SRC) -o $(OUT) $(CFLAGS)
run: $(OUT)
	$(OUT)
clean:
	rm bin/*.exe
