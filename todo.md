# AGFX Development Todo List

Track progress on the AGFX 3D graphics library implementation.

## ✅ Completed

- [ ] Initial design and architecture planning

## 🚧 In Progress

## 📋 To Do

### Phase 1: Foundation
- [X] Update build system for cgltf
- [ ] Create core types and math
- [ ] Implement camera system
- [ ] Basic rendering (shapes only)

### Phase 2: Model Loading
- [ ] Integrate cgltf library
- [ ] Parse glTF meshes and materials
- [ ] Create mesh rendering pipeline
- [ ] Add material system

### Phase 3: Advanced Features
- [ ] Scene graph with node hierarchy
- [ ] Camera modes and controls
- [ ] Lighting system (simple directional)
- [ ] Debug tools (wireframe, bounding boxes)

### Phase 4: Polish
- [ ] Animation support
- [ ] Performance optimizations
- [ ] Documentation and examples
- [ ] Testing with various glTF models

### Next Steps (Immediate)
- [ ] Add cgltf to vendor directory
- [ ] Create `agfx.h` with type definitions
- [ ] Implement basic rendering functions
- [ ] Add glTF loader skeleton
- [ ] Create test program

### Success Criteria
- [ ] Load a simple glTF model (cube/sphere)
- [ ] Render it with proper transforms
- [ ] Support basic camera movement
- [ ] Clean up resources automatically
- [ ] Maintain ~60 FPS with simple scenes
- [ ] Document API clearly

## 🎯 Future Considerations

### Questions to Resolve
- [ ] Target complexity: Single model vs. multi-model scenes?
- [ ] Camera needs: Simple orbit vs. full FPS controls?
- [ ] Material depth: Basic color/texture vs. full PBR?
- [ ] Performance: Priority on simplicity or efficiency?
- [ ] Animation: Skeletal animation needed from start?

### Dependencies
- [ ] cgltf (glTF loader) - Need to add
- [ ] stb_image (texture loading) - Optional integration
- [ ] miniaudio (future audio support) - Optional
