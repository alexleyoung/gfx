.PHONY: all clean

all: run

build: build/main

build/main: main.c
	zig cc main.c -o build/main -lX11 -lXi -lXcursor -lGL -lasound -ldl -lm

run: clean build shader
	./build/main

vendor: vendor/sokol

vendor/sokol:
	test -d vendor || mkdir -p vendor
	git clone https://github.com/floooh/sokol.git vendor/sokol

clean-vendor:
	rm -rf vendor/sokol

shader:
	./sokol-shdc --input assets/shaders/triangle.glsl --output triangle_shader.h --slang glsl430:hlsl5:metal_macos

clean:
	rm -f build/main
