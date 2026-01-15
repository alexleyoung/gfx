.PHONY: all clean

all: build

build: build/main

build/main: main.c
	zig cc main.c -o build/main -lX11 -lXi -lXcursor -lGL -lasound -ldl -lm

run: build
	./build/main

clean:
	rm -f build/main
