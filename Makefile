.PHONY: all clean

all: build

build: build/main

build/main: main.c
	zig cc main.c -o build/main -lX11 -lXi -lXcursor -lGL -lasound -ldl -lm

run: build
	./build/main

vendor: vendor/sokol

vendor/sokol:
	test -d vendor || mkdir -p vendor
	git clone https://github.com/floooh/sokol.git vendor/sokol

clean-vendor:
	rm -rf vendor/sokol

clean:
	rm -f build/main
