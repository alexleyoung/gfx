.PHONY: all clean

all: run

build: build/main

build/main: $(wildcard src/*.c)
	zig cc $(wildcard src/*.c) -o build/main -lX11 -lXi -lXcursor -lGL -lasound -ldl -lm -Iinclude -Ivendor/cglm/include

run: clean build shader
	./build/main

vendor: vendor/sokol vendor/cglm

vendor/sokol:
	test -d vendor || mkdir -p vendor
	git clone https://github.com/floooh/sokol.git vendor/sokol

vendor/cglm:
	test -d vendor || mkdir -p vendor
	git clone https://github.com/recp/cglm.git vendor/cglm

clean-vendor:
	rm -rf vendor/sokol
	rm -rf vendor/cglm

shader:
	./sokol-shdc --input assets/shaders/triangle.glsl --output include/triangle_shader.h --slang glsl430:hlsl5:metal_macos

clean:
	rm -f build/main
