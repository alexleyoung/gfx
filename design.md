# agfx - alex's graphics (framework)

> "A Raylib-inspired 3D rendering engine for C with glTF support"

## Overview

AGFX is a lightweight 3D rendering library designed to make it easy to load and render 3D models (primarily glTF format) using modern OpenGL with Sokol. It follows Raylib's design philosophy: simple API, automatic resource management, and focus on getting things on screen quickly.

## Design Philosophy

1. **Simplicity First**: Single-header library, intuitive function names
2. **Resource Management**: Load/Unload pattern with automatic cleanup
3. **glTF-Centric**: Primary focus on glTF 2.0 format
4. **Sokol Backend**: Leverages existing Sokol graphics foundation
5. **C-First**: Plain C99, no C++ required

## Architecture

### High-Level Structure

```
src/
├── agfx.c           # Core API implementation
├── agfx_math.c      # Vector/Matrix math (cglm wrapper)
├── agfx_render.c    # Rendering backend (Sokol)
├── agfx_loader.c    # glTF loader (cgltf)
├── agfx_camera.c    # Camera systems
├── agfx_models.c    # Model/Mesh handling
└── agfx_utils.c     # Utilities, logging, file I/O

include/
├── agfx.h           # Public API header
├── agfx_math.h      # Math types and functions
└── agfx_types.h     # Core type definitions
```

### Component Breakdown

#### 1. Core Types (`agfx_types.h`)
- **Vector2/3/4**: Position, rotation, scale
- **Matrix**: 4x4 transformation matrix
- **Color**: RGBA color structure
- **Texture**: GPU texture handle + metadata
- **Material**: Properties (color, metallic, roughness, textures)
- **Mesh**: Vertex data (positions, normals, UVs) + GPU buffers
- **Model**: Collection of meshes + materials + transforms
- **Camera**: View/projection parameters
- **Scene**: Node hierarchy for complex models

#### 2. Math Module (`agfx_math.h`)
- Wraps cglm for performance
- Provides familiar Raylib-style functions
- Handles coordinate system conversions
- **Example**: `Vector3Add()`, `MatrixMultiply()`, `QuaternionSlerp()`

#### 3. Rendering Backend (`agfx_render.c`)
- **Sokol-based**: Uses existing sokol_gfx.h
- **Shader system**: Default PBR shader + material-specific overrides
- **Resource management**: Automatic GPU resource cleanup
- **Batching**: Basic material-based grouping (future: advanced batching)

#### 4. glTF Loader (`agfx_loader.c`)
- **cgltf integration**: Parse glTF files
- **Mesh extraction**: Convert glTF primitives to AGFX meshes
- **Material parsing**: PBR metal/roughness workflow
- **Texture loading**: Embedded and external textures
- **Scene hierarchy**: Convert glTF nodes to AGFX scene graph

#### 5. Camera System (`agfx_camera.c`)
- **Multiple modes**: Free, orbital, first-person, third-person
- **Input handling**: Mouse/keyboard integration
- **Matrix generation**: View and projection matrices
- **Smooth movement**: Lerp/lerp for camera transitions

#### 6. Model Management (`agfx_models.c`)
- **Mesh generation**: Cube, sphere, plane, cylinder
- **Model transforms**: Position, rotation, scale
- **Bounding boxes**: For culling and physics
- **Instancing**: Efficient rendering of repeated objects

## API Design Examples

### Basic Usage Pattern

```c
#include "agfx.h"

int main() {
    // Initialize
    InitWindow(1280, 720, "AGFX Demo");
    
    // Load resources
    Camera camera = CreateCamera(
        (Vector3){0, 2, 5},    // position
        (Vector3){0, 0, 0},    // target
        (Vector3){0, 1, 0},    // up
        45.0f                  // FOV
    );
    
    Model model = LoadModel("assets/models/character.gltf");
    
    // Render loop
    while (!WindowShouldClose()) {
        // Update
        UpdateCamera(&camera);
        
        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        BeginMode3D(camera);
        DrawModel(model, Vector3Zero(), 1.0f, WHITE);
        DrawGrid(10, 1.0f);
        EndMode3D();
        
        EndDrawing();
    }
    
    // Cleanup
    UnloadModel(model);
    CloseWindow();
    return 0;
}
```

### Resource Management

```c
// Load
Model model = LoadModel("scene.gltf");
Material material = LoadMaterialDefault();

// Modify
SetModelMaterial(&model, 0, material);
SetMaterialTexture(&material, MAP_DIFFUSE, texture);

// Draw
DrawModel(model, position, scale, tint);

// Unload - automatically frees all GPU resources
UnloadModel(model);
```

### Camera Controls

```c
// Setup camera with mode
Camera camera = CreateCamera(...);
SetCameraMode(camera, CAMERA_FREE);  // or CAMERA_ORBITAL, CAMERA_FIRST_PERSON

// Update handles input automatically
UpdateCamera(&camera);

// Get matrices for custom rendering
Matrix view = GetCameraMatrix(camera);
Matrix proj = GetCameraProjection(camera, aspect);
```

### Mesh Generation

```c
// Built-in shapes
Mesh cube = GenMeshCube(1.0f, 1.0f, 1.0f);
Mesh sphere = GenMeshSphere(1.0f, 16, 16);
Mesh plane = GenMeshPlane(10.0f, 10.0f);

// Create model from mesh
Model model = LoadModelFromMesh(cube);
```

## Key Features

### 1. glTF 2.0 Support
- **Mesh loading**: Vertices, indices, normals, UVs
- **Materials**: PBR metal/roughness workflow
- **Textures**: Embedded base64 and external files
- **Hierarchy**: Node transforms and parenting
- **Animations**: Basic skeletal animation support

### 2. Rendering Pipeline
```
Input → Camera → Transform → Shading → Output
          ↓           ↓          ↓
       View Matrix  Model     Material
       Proj Matrix  Mesh      Shader
```

### 3. Shader System
- **Default shader**: Basic PBR lighting
- **Material overrides**: Custom per-material shaders
- **Uniform management**: Automatic matrix passing

### 4. Resource Tracking
- **Automatic cleanup**: Reference counting
- **Error handling**: Graceful fallbacks
- **Logging**: Optional debug output

## Dependencies

### Required
- **Sokol** (graphics backend) - Already in vendor/
- **cglm** (math) - Already in vendor/
- **cgltf** (glTF loader) - Need to add

### Optional
- **stb_image** (texture loading) - Could integrate
- **miniaudio** (future audio support)

## Integration with Current Code

Your existing codebase provides:
- Sokol setup and initialization
- Shader compilation pipeline
- Basic vertex buffer management
- Camera math (using cglm)

AGFX will:
- **Abstract** these into clean API functions
- **Enhance** with glTF loading
- **Simplify** model management
- **Expand** with camera controls
