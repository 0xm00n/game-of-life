# Game of Life - Implementation Roadmap

## Project Overview
A high-performance C++ implementation of Conway's Game of Life and other cellular automata, exploring complexity, emergence, and self-replication. The goal is to demonstrate phenomena like GoL-in-GoL (meta-cellular automata) and Rule 110 Turing completeness.

## System Specs
- **CPU**: Intel i9-13900H (6 P-cores @ 5.4GHz, 8 E-cores @ 4.1GHz, 20 threads)
- **RAM**: 32GB DDR4/DDR5
- **GPU**: NVIDIA RTX 3050 Laptop (6GB VRAM, CUDA 13.0)
- **Display**: 2K (2560x1440)
- **OS**: Ubuntu 24.04.3, X11

## Tech Stack
- **Language**: C++23
- **Build System**: CMake 3.20+
- **Graphics**: OpenGL 4.6, GLFW 3, GLEW
- **GPU Compute**: CUDA 13.0 (for Phase 4)
- **Math Library**: GLM
- **Testing**: Google Test

## Perf Targets
- **Frame Rate**: 60 FPS @ 2560x1440
- **Active Cells**: 10M+ cells with GPU acceleration
- **VRAM Usage**: <2GB

---

## Phase 1: Foundation (COMPLETED)

### Completed Components
- CMake project with C++23 standard
- `Grid` class: bounded 2D grid with `std::vector<uint8_t>` storage
- `Simulator` class: double-buffered, Conway's rules, Moore neighborhood
- Cache-friendly iteration (y outer, x inner loop)

### Conway's Game of Life Rules
1. Live cell with 2-3 neighbors survives
2. Live cell with <2 or >3 neighbors dies
3. Dead cell with exactly 3 neighbors becomes alive

---

## Phase 2: Unit Testing (COMPLETED)

### Completed Components
- Google Test integration with CMake FetchContent
- Grid tests: dimensions, set/get cells, clear, boundaries
- Simulator tests: neighbor counting, Conway rules, pattern tests
- Code coverage: 100% line and function coverage (lcov)

---

## Phase 3: Visualization (COMPLETED)

### Completed Components
- **Window class**: GLFW window, OpenGL 4.6 context, input handling
- **Shader class**: GLSL compilation, uniform setting
- **Renderer class**: VAO/VBO setup, instanced rendering
- **Camera class**: orthographic projection, pan (WASD), zoom (Q/E)
- **Grid overlay**: shader-based grid lines
- **Simulation controls**: pause (Space), step (Right), speed (Up/Down)
- **UI**: generation counter and speed in window title

### Controls
| Key | Action |
|-----|--------|
| WASD | Pan camera |
| Q/E | Zoom out/in |
| Space | Pause/resume |
| Right | Step once |
| Up/Down | Adjust speed |

### Files Created
```
include/rendering/
├── Window.h
├── Shader.h
├── Renderer.h
└── Camera.h
src/rendering/
├── Window.cpp
├── Shader.cpp
├── Renderer.cpp
└── Camera.cpp
```

---

## Phase 4: Performance Optimization

### Goals
Scale to millions of active cells with GPU acceleration and multi-threading.

### Approaches
- **Sparse grid**: `std::unordered_map` for infinite grid simulation
- **CUDA compute**: GPU kernels for Conway's rules
- **Multi-threading**: tile-based parallelism with `std::jthread`
- **SIMD**: AVX2 intrinsics for vectorized neighbor counting

### Performance Targets
- 5-10M+ active cells @ 60 FPS
- Full CPU/GPU utilization

---

## Phase 5: Multi-Scale Architecture

### Goals
Hierarchical grid structure supporting infinite zoom and meta-cellular automata.

### Components
- Quadtree for spatial subdivision
- Level-of-detail (LOD) system
- Seamless zoom transitions

---

## Phase 6: Pattern Library & Tools

### Goals
Pattern loading, editing, and a curated collection.

### Components
- RLE and plaintext pattern parsers
- Mouse-based cell editing
- Pattern library (still lifes, oscillators, spaceships, guns)

---

## Phase 7: Rule 110 (1D Cellular Automaton)

### Goals
Implement Rule 110 to demonstrate Turing completeness.

### Rule 110 Encoding
```
111 110 101 100 011 010 001 000  (neighborhoods)
 0   1   1   0   1   1   1   0   (next state)
```

---

## Phase 8: Self-Replication (Meta-GoL)

### Goals
Game of Life simulating itself using OTCA metapixels.

### Challenges
- Scale: 10,000x10,000+ cells per metapixel
- Requires sparse grid and multi-scale rendering from earlier phases

---

## Current Project Structure

```
game-of-life/
├── CMakeLists.txt
├── README.md
├── spec.md
├── include/
│   ├── core/
│   │   └── Grid.h
│   ├── simulation/
│   │   └── Simulator.h
│   └── rendering/
│       ├── Window.h
│       ├── Shader.h
│       ├── Renderer.h
│       └── Camera.h
├── src/
│   ├── main.cpp
│   ├── core/
│   │   └── Grid.cpp
│   ├── simulation/
│   │   └── Simulator.cpp
│   └── rendering/
│       ├── Window.cpp
│       ├── Shader.cpp
│       ├── Renderer.cpp
│       └── Camera.cpp
├── tests/
│   ├── test_grid.cpp
│   └── test_sim.cpp
└── build/
```

---

## References

- [LifeWiki](https://conwaylife.com/wiki/) - Pattern database
- [Golly](http://golly.sourceforge.net/) - Reference implementation
- [LearnOpenGL](https://learnopengl.com/) - OpenGL tutorial
- [OTCA Metapixel](https://conwaylife.com/wiki/OTCA_metapixel)

---

## Current Status

- **Phase 1**: COMPLETED
- **Phase 2**: COMPLETED
- **Phase 3**: COMPLETED
- **Phase 4**: PENDING
- **Phase 5**: PENDING
- **Phase 6**: PENDING
- **Phase 7**: PENDING
- **Phase 8**: PENDING
