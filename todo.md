# AGFX Development Checkpoints

A series of checkpoints for building a 3D FPS game engine from scratch.

---

## 🚦 Phase 1: Foundation (Weeks 1-2)

*Goal: Get a window open and basic rendering working.*

### 1.1 Project Setup
- [ ] Set up build system (Makefile or CMake)
- [ ] Integrate Sokol graphics library
- [ ] Create basic `main.c` with window
- [ ] Clear screen with solid color

### 1.2 Core Types
- [ ] Define `Vec3` struct (x, y, z as float)
- [ ] Define `AABB` struct (min, max vectors)
- [ ] Define `Color` struct (r, g, b)
- [ ] Basic vector operations: add, subtract, scale

### 1.3 Math Utilities
- [ ] Wrap cglm Vec3 functions
- [ ] Matrix identity and perspective
- [ ] Vector magnitude and normalize
- [ ] Cross product and dot product

### 1.4 Basic Render Loop
- [ ] Frame time measurement (`dt`)
- [ ] 60 FPS target with basic timing
- [ ] Debug logging for FPS

**Checkpoint criteria:**
- Window opens with solid background
- FPS counter showing ~60
- Clean exit with window close

---

## 🚦 Phase 2: Camera System (Weeks 2-3)

*Goal: Build an FPS camera with free-look and WASD movement.*

### 2.1 Camera Structure
- [ ] Define `Camera` struct
  - Position (vec3)
  - Yaw (float, horizontal)
  - Pitch (float, vertical)
  - FOV (float)
  - Near/far planes
- [ ] Define `CameraInput` struct
  - Mouse delta
  - Key state (WASD)

### 2.2 Input Handling
- [ ] Mouse capture and delta calculation
  - Hide cursor in window
  - Accumulate mouse movement
  - Reset cursor to center each frame
- [ ] Keyboard state management
  - Check WASD keys per frame
  - Store input state for update

### 2.3 Camera Update
- [ ] Update yaw from mouse X
- [ ] Update pitch from mouse Y (clamp ±89°)
- [ ] Convert yaw/pitch to forward vector
  - Forward = `(cos(yaw)*cos(pitch), sin(pitch), sin(yaw)*cos(pitch))`
  - Right = `(cos(yaw-90°), 0, sin(yaw-90°))`
  - Up = cross(right, forward)
- [ ] Move camera based on WASD input
  - Forward/Back: `position += forward * speed`
  - Strafe left/right: `position += right * speed`

### 2.4 View/Projection Matrix
- [ ] Generate view matrix (lookAt)
- [ ] Generate perspective projection matrix
- [ ] Pass matrices to render pipeline

**Checkpoint criteria:**
- Mouse locked to window
- Free look with pitch clamping
- WASD moves camera in 3D
- Can look up/down and around
- Clean camera code with clear separation

---

## 🚦 Phase 3: Physics (Weeks 3-5)

*Goal: Implement swept AABB collision with gravity and jumping.*

### 3.1 AABB & Swept Collision
- [ ] Define `AABB` struct with `min` and `max` (vec3)
- [ ] AABB overlap test
  ```c
  bool aabb_overlaps(AABB a, AABB b) {
      return a.min.x < b.max.x && a.max.x > b.min.x &&
             a.min.y < b.max.y && a.max.y > b.min.y &&
             a.min.z < b.max.z && a.max.z > b.min.z;
  }
  ```
- [ ] Swept AABB implementation
  - Cast ray along movement vector
  - Find earliest collision time
  - Return `t` value (0-1) for collision point

### 3.2 Player Physics
- [ ] Define `Player` struct
  - Position (vec3)
  - Velocity (vec3)
  - Size (vec3) - half-extents
  - Grounded (bool)
  - Can jump (bool)
- [ ] Gravity application
  - `velocity.y -= gravity * dt`
  - Terminal velocity clamping
- [ ] Jumping
  - `if (grounded && jump_pressed) { velocity.y = jump_force; }`

### 3.3 Collision Response
- [ ] Resolve single AABB collision
  - Sweep player AABB against target AABB
  - If collision, push out along smallest penetration axis
  - Zero velocity component in that axis
- [ ] Iterative resolution (multiple boxes)
  - Collision checks against all map boxes
  - Resolve in order: X, Y, Z (or by penetration depth)

### 3.4 Ground Detection
- [ ] Check if player is on ground
  - Test `position.y - size.y` against floor height
  - Or check collision below player
- [ ] Step-up logic
  - If movement blocked by small obstacle, lift player
  - Allow walking up 0.3-unit steps without jumping

**Checkpoint criteria:**
- Player falls with gravity
- Player stops at walls (no clipping)
- Player can jump
- Player can walk up stairs/ledges
- Smooth wall sliding (not sticking)

---

## 🚦 Phase 4: Map System (Weeks 5-6)

*Goal: Define and load a text-based map format.*

### 4.1 Map Format Design
- [ ] Define text format:
  ```
  # Comments
  box 0 0 0 10 2 20 128 128 128  # Floor (x y z w h d r g b)
  box -5 0 5 2 4 2 200 50 50     # Pillar
  player_start 0 3 0             # Spawn point
  ```
- [ ] Define binary format (for optimization later)
  - Header: magic, version, num_boxes
  - Data: each box (pos + size + color)

### 4.2 Map Parser
- [ ] File reading functions
  - Read entire file into buffer
  - Parse line by line
  - Skip comments and empty lines
- [ ] Token parsing
  - Split line into tokens
  - Parse numbers (floats, ints)
  - Handle errors gracefully
- [ ] Box parsing
  - `box x y z w h d r g b`
  - Validate dimensions (>0)
  - Store in dynamic array

### 4.3 Map Builder (In-Engine)
- [ ] Map construction API
  ```c
  Map* map = MapCreate();
  MapAddBox(map, (vec3){0,0,0}, (vec3){10,2,20}, (Color){128,128,128});
  MapSave(map, "level1.txt");
  ```
- [ ] Dynamic array for boxes
  - Allocate initial capacity
  - Grow as needed
  - Free on unload

### 4.4 Map Save/Load
- [ ] Save to text format
  - Write header comment
  - Write each box line
- [ ] Load from text format
  - Parse into Map struct
  - Validate data
- [ ] Collision geometry generation
  - Each box becomes an AABB for physics
  - Store alongside rendered geometry

**Checkpoint criteria:**
- Text file loads into in-memory map
- Map saves back to text file (round-trip)
- Collision boxes match rendered boxes
- Can add/remove boxes at runtime

---

## 🚦 Phase 5: Integration (Weeks 6-7)

*Goal: Connect camera, physics, and map into a playable demo.*

### 5.1 Rendering Pipeline
- [ ] Render map boxes (flat shading)
  - Each box → 12 triangles (6 faces × 2)
  - Single color per box (no texture)
  - Pass MVP matrices to shader
- [ ] Player rendering (simple)
  - Render player AABB as wireframe overlay
  - Or render as colored cube
- [ ] Render order
  - Sky/background
  - Map geometry
  - Player/overlays

### 5.2 Shader Setup
- [ ] Simple vertex shader
  - Transform: `gl_Position = MVP * vec4(position, 1.0)`
  - Pass color to fragment shader
- [ ] Simple fragment shader
  - Solid color: `out_color = color;`
  - Or basic lighting: diffuse + ambient

### 5.3 Game Loop Integration
- [ ] Order of operations per frame:
  ```
  1. Get input
  2. Update camera (from input)
  3. Update player physics
  4. Resolve collisions
  5. Render frame
  ```
- [ ] Delta time handling
  - Variable timestep with accumulator
  - Fixed physics tick (60 FPS target)

### 5.4 In-Game Map Editor (Basic)
- [ ] Toggle edit mode (key press)
- [ ] Place new box at camera position + forward
  - Press 'E' to add box
  - Box size from keyboard input
  - Color selection (1-8 keys)
- [ ] Save level to file

**Checkpoint criteria:**
- Running game with visible map
- Player can move through level with FPS controls
- Collision works with loaded map
- Can place boxes during gameplay
- Can save level and reload it

---

## 🚦 Phase 6: Polish & Demo (Weeks 7-8)

*Goal: Create a playable FPS demo showing all features.*

### 6.1 Gameplay Features
- [ ] Multiple levels
  - Start room
  - Narrow corridors
  - Stairs/ledges
  - Floating platforms
- [ ] Start position marker
  - Defined in map file
  - Player spawns there on level load

### 6.2 UI Elements
- [ ] Crosshair (center of screen)
- [ ] Player position debug (X, Y, Z)
- [ ] Velocity debug display
- [ ] FPS counter (already done in phase 1)

### 6.3 Quality of Life
- [ ] Input smoothing (prevent camera jitter)
- [ ] Collision bug fixes (sticky corners, edge cases)
- [ ] Performance: culling (frustum or simple distance)
- [ ] Debug camera (noclip mode)

### 6.4 Demonstration
- [ ] Sample levels built with the engine
  - Box-only construction
  - Various heights and arrangements
- [ ] README with screenshots
- [ ] Example level files

**Checkpoint criteria:**
- Complete playable demo
- Multiple levels loadable
- Can create levels entirely within engine
- Clean code with comments
- README explaining how to play

---

## 🎯 Future Extensions

After completing the base engine:

### Extension 1: Entity System
- [ ] Component-based entity struct
- [ ] Entity registry (dynamic array)
- [ ] Basic entity update/render loop

### Extension 2: Enemy AI
- [ ] State machine (idle, chase, attack, flee)
- [ ] Simple pathfinding (A* on grid)
- [ ] Enemy collision/physics

### Extension 3: Combat
- [ ] Weapon system (raycast-based shooting)
- [ ] Health and damage
- [ ] Ammo management

### Extension 4: Heightfield Terrain
- [ ] Replace AABB collision with heightfield
- [ ] Slopes and ramps
- [ ] Terrain editing

### Extension 5: Textures & Materials
- [ ] Texture loading (stb_image)
- [ ] UV mapping
- [ ] Basic lighting (per-vertex)

---

## Success Metrics

### Must Have
- [ ] FPS camera with free look
- [ ] AABB collision with gravity and jumping
- [ ] Text-based map format
- [ ] In-engine level builder
- [ ] Render solid-colored boxes
- [ ] Playable demo

### Stretch Goals
- [ ] Multiple levels
- [ ] Enemy AI (FSM)
- [ ] Weapons/shooting
- [ ] Heightfield terrain
- [ ] Audio (basic)

### Educational Value
- [ ] Each system is self-contained and understandable
- [ ] Code has comments explaining "why", not just "what"
- [ ] Can modify one system without breaking others
- [ ] Demonstrates common game engine patterns

---

## Next Immediate Steps

1. Review this checkpoint list
2. Start Phase 1 (Foundation)
3. Report back with questions or to mark Phase 1 complete
