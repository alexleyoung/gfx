set shell := ["bash", "-uc"]

# List all available recipes
all: run

# Build the executable
build: build_main

build_main: vendor
	mkdir -p build
	clang src/*.c -o build/main -lX11 -lXi -lXcursor -lGL -lasound -ldl -lm -Iinclude -Ivendor/cglm/include

# Clean, build, generate shaders, and run
run: clean shader build
	./build/main

# Set up vendor dependencies
vendor: sokol cglm

sokol:
	test -d vendor || mkdir -p vendor
	test -d vendor/sokol || git clone https://github.com/floooh/sokol.git vendor/sokol

cglm:
	test -d vendor || mkdir -p vendor
	test -d vendor/cglm || git clone https://github.com/recp/cglm.git vendor/cglm

# Clean vendor directories
clean-vendor:
	rm -rf vendor/sokol
	rm -rf vendor/cglm

# Generate shader headers
shader: sokol
	test -x ./sokol-shdc || (mkdir -p vendor && curl -L https://github.com/floooh/sokol-tools-bin/raw/master/bin/linux/sokol-shdc -o sokol-shdc && chmod +x sokol-shdc)
	./sokol-shdc --input assets/shaders/triangle.glsl --output include/triangle_shader.h --slang glsl430:hlsl5:metal_macos

# Clean build artifacts
clean:
	rm -f build/main
