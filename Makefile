CC = gcc
COMPILER_FLAGS = -std=c99 -Wall -Wextra -O3
LINKER_FLAGS = `sdl2-config --cflags --libs`

all: demo

demo: src/demo.c
	$(CC) $(COMPILER_FLAGS) -o build/$@ $^ $(LINKER_FLAGS)

run: demo
	./build/demo
