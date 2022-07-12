CC = gcc
COMPILER_FLAGS = -std=c99 -Wall -Wextra -fno-builtin -O3
LINKER_FLAGS = -lxcb -lxcb-image

all: demo_linux

demo_linux: src/demo_linux.c
	$(CC) $(COMPILER_FLAGS) -o build/$@ $^ $(LINKER_FLAGS)

run: demo_linux
	./build/demo_linux
