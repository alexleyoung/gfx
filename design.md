# agfx

> a 3D game engine for FPS-style games

## Design Philosophy

1. **Fundamentals First**: Every system teaches a core concept. No black boxes.
2. **From Scratch**: Implement physics, collision, and rendering yourself. Learn by building.
3. **Simple Geometries**: Boxes only (for now). Simple enough to understand in an afternoon.
4. **Game-Ready**: Designed to build actual games, not just render scenes.
5. **C-First**: Plain C99. No C++, no external tools required.

## Core Capabilities

### 1. Physics System: Swept AABB
- Collision detection before impact (swept)
- Wall sliding when pushing into surfaces
- Gravity and jumping support
- Ground detection for stairs/ledges

**What it teaches**: Collision detection, game loops, vector math

### 2. FPS Camera
- Mouse-captured free-look (pitch + yaw)
- WASD + strafing movement
- Natural FPS controls (no gimbal lock)

**What it teaches**: View matrices, input handling, Euler angles

### 3. Map System (Engine-Built)
- Text-based format: `box x y z w h d color`
- Parse at runtime, no external tools
- Auto-generate collision from boxes
- Load/save levels from disk

**What it teaches**: File I/O, parsing, scene graph basics

### 4. Solid-Color Rendering
- No textures, no PBR (for now)
- Flat shading per brush
- Fast and clean rendering pipeline

**What it teaches**: Vertex buffers, graphics pipeline, shaders

## Architecture

```
src/
├── agfx.c            # Public API implementation
├── agfx_physics.c    # Swept AABB collision
├── agfx_camera.c     # FPS camera controller
├── agfx_map.c        # Map parser & builder
├── agfx_render.c     # Sokol renderer
├── agfx_math.c       # cglm wrapper
└── agfx_utils.c      # File I/O, logging

include/
├── agfx.h            # Public API
├── agfx_types.h      # Core types
├── agfx_physics.h    # Physics types
├── agfx_camera.h     # Camera API
└── agfx_map.h        # Map API
```

## Resource Management

All resources follow manual Load/Unload pattern for explicit control:

```c
Map* map = LoadMap("level1.txt");
Camera camera = CameraCreate(/*...*/);
while (running) {
    CameraUpdate(&camera, dt);
    PhysicsUpdate(player, map, dt);
    RenderFrame(camera, map);
}
UnloadMap(map);
```

## Dependencies

- **Sokol** (graphics backend) ✓
- **cglm** (math) ✓
- **Custom file format** (map data)

## Example Usage

```c
#include "agfx.h"

int main() {
    // Initialize
    agfx_init((agfx_desc){
        .width = 1280,
        .height = 720,
        .title = "AGFX FPS Demo"
    });

    // Build or load level
    Map* map = MapCreate();
    AddBox(map, (vec3){0,0,0}, (vec3){10,2,20}, (Color){80,80,80});
    AddBox(map, (vec3){5,2,8}, (vec3){2,2,2}, (Color){200,50,50});
    MapSave(map, "level1.txt");

    // Player setup
    Player player = {
        .position = {0, 3, 0},
        .velocity = {0, 0, 0},
        .size = {0.5f, 1.8f, 0.5f}  // Doom guy proportions
    };

    Camera camera = CameraCreate(&player);

    // Game loop
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        // Input → Camera → Physics
        CameraUpdate(&camera, dt);
        PlayerUpdate(&player, &camera, dt);
        PhysicsCollide(&player, map, dt);

        // Render
        BeginDrawing();
        ClearBackground(SKY);
        BeginMode3D(&camera);
        RenderMap(map);
        RenderPlayer(&player);
        EndMode3D();
        EndDrawing();
    }

    // Cleanup
    UnloadMap(map);
    agfx_shutdown();
}
```

## Roadmap

### Phase 1: Foundation
Project setup, core types, render loop

### Phase 2: Camera
FPS camera with mouse control

### Phase 3: Physics
Collision detection and response

### Phase 4: Map System
Level format and loading

### Phase 5: Integration
Putting it all together

### Future
- Entity system
- Enemy AI (FSM)
- Weapons
- Heightfield terrain
- Textures

This is a teaching engine. The goal is understanding, not convenience.
