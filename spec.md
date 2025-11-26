# Game of Life - Implementation Roadmap

## Project Overview
A high-performance C++ implementation of Conway's Game of Life and other cellular automata, exploring complexity, emergence, and self-replication. The goal is to demonstrate phenomena like GoL-in-GoL (meta-cellular automata) and Rule 110 Turing completeness with cool ass visualization.

## System Specs
- **CPU**: Intel i9-13900H (6 P-cores @ 5.4GHz, 8 E-cores @ 4.1GHz, 20 threads)
- **RAM**: 32GB DDR4/DDR5
- **GPU**: NVIDIA RTX 3050 Laptop (6GB VRAM, CUDA 13.0)
- **Display**: 2K (2560x1440)
- **OS**: Ubuntu 24.04.3, X11

## Tech Stack
- **Language**: C++23
- **Build System**: CMake 3.20+
- **Graphics**: Vulkan 1.3+ (or CUDA + OpenGL for GPU compute)
- **Math Library**: GLM 1.0+
- **Testing**: Google Test (latest)
- **Threading**: C++23 `std::jthread`, parallel algorithms
- **SIMD**: AVX2 intrinsics (standard on Raptor Lake)
- **Window Management**: GLFW 3.4+

## Perf Targets
- **VRAM Usage**: <2GB
- **Frame Rate**: 60 FPS @ 2560x1440
- **Active Cells**: 10M+ cells with GPU acceleration
- **Simulation Speed**: Real-time updates with full CPU/GPU utilization

---

## Phase 1: Foundation COMPLETED

### Goals
Basic bounded Conway's Game of Life with CPU simulation and terminal output.

### Completed Components
- CMake project with C++23 standard
- Project directory structure (`include/`, `src/`, `tests/`)
- `Grid` class (bounded 2D grid)
  - Fixed-size grid with `std::vector<uint8_t>` storage
  - 1D array representation: `index = y * width + x`
  - Methods: `is_alive()`, `set_cell()`, `clear()`, `get_width()`, `get_height()`
- `Simulator` class
  - Double-buffered grids for correct state updates
  - `count_neighbors()` using Moore neighborhood (8 neighbors)
  - `step()` implementing Conway's rules
  - Cache-friendly iteration (y outer, x inner loop)
- Working blinker pattern test

### Conway's Game of Life Rules
1. Any live cell with 2 or 3 neighbors survives
2. Any live cell with <2 or >3 neighbors dies
3. Any dead cell with exactly 3 neighbors becomes alive
4. All other cells stay dead

### Files Created
```
game-of-life/
├── CMakeLists.txt
├── include/
│   ├── core/
│   │   └── Grid.h
│   └── simulation/
│       └── Simulator.h
├── src/
│   ├── main.cpp
│   ├── core/
│   │   └── Grid.cpp
│   └── simulation/
│       └── Simulator.cpp
└── build/
```

---

## Phase 2: Unit Testing (CURRENT)

### Goals
Automated testing infrastructure using Google Test.

### Test Suites to Implement

#### Grid Tests (`tests/test_grid.cpp`)
- Constructor initializes correct dimensions
- All cells start dead
- `set_cell()` and `is_alive()` work correctly
- `clear()` resets all cells to dead
- Boundary cells accessible
- Index calculation correct (`get_index()`)

#### Simulator Tests (`tests/test_simulator.cpp`)
- Constructor creates grids with correct dimensions
- `count_neighbors()` accuracy:
  - Center cell (all 8 neighbors accessible)
  - Edge cells (5-7 neighbors)
  - Corner cells (3 neighbors)
  - Out-of-bounds treated as dead
- `step()` applies Conway's rules correctly:
  - Survival (2-3 neighbors)
  - Death (underpopulation/overpopulation)
  - Birth (exactly 3 neighbors)
- Double buffering works (no in-place update issues)
- Pattern tests (still lifes, oscillators, gliders)

#### Integration Tests
- Multi-step evolution matches expected results
- Large grid performance (2048x2048)
- Memory usage within bounds

### CMake Integration
```cmake
# Add Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()
add_executable(tests
    tests/test_grid.cpp
    tests/test_simulator.cpp
    src/core/Grid.cpp
    src/simulation/Simulator.cpp
)
target_link_libraries(tests GTest::gtest_main)
include(GoogleTest)
gtest_discover_tests(tests)
```

### Success Criteria
- All tests pass
- Code coverage >80%
- No memory leaks (valgrind/sanitizers)

---

## Phase 3: Visualization (Vulkan + GLFW)

### Goals
Real-time graphical rendering with interactive camera controls.

### Components to Implement

#### Window Management
- GLFW window creation (2560x1440 or windowed)
- Input handling (keyboard, mouse)
- Event loop

#### Vulkan Renderer
- Vulkan instance, device, swapchain setup
- Graphics pipeline for cell rendering
- Instanced rendering (one draw call for all cells)
- Vertex buffer for quad (2 triangles per cell)
- Fragment shader for cell coloring
- Efficient buffer updates (only changed regions)

#### Camera System
- 2D orthographic projection
- Pan (WASD or arrow keys)
- Zoom (mouse wheel)
- Smooth interpolation for camera movement

#### Rendering Optimizations
- **Frustum culling**: Only render visible cells
- **Dynamic vertex buffers**: Stream only visible/changed cells
- **Instanced rendering**: Minimize draw calls
- **Grid overlay** (optional): Show cell boundaries at high zoom

#### Visual Effects
- Color schemes (grayscale, heat map, age-based)
- Cell trails/fading (cells dim over time)
- Grid lines at appropriate zoom levels
- Smooth animations between generations

### Performance Targets
- 60 FPS @ 2K resolution
- <1GB VRAM usage
- Support 100K+ visible cells

### Success Criteria
- Smooth 60 FPS with large grids
- Responsive camera controls
- Stable memory usage
- No visual artifacts or tearing

---

## Phase 4: Performance Optimization

### Goals
Scale to millions of active cells with GPU acceleration and multi-threading.

### Sparse Grid Implementation
- Replace dense grid with `std::unordered_map<std::pair<int,int>, uint8_t>`
- Spatial hashing for fast neighbor queries
- Only store live cells and their immediate dead neighbors
- Automatic expansion (infinite grid simulation)

### GPU Acceleration (CUDA)
- CUDA compute kernels for GoL rules
- Grid state on GPU memory
- CPU-GPU synchronization
- Fallback to CPU for edge cases

### Multi-threading (CPU)
- Tile-based parallelism (divide grid into chunks)
- Work-stealing scheduler (C++23 `std::jthread`)
- Lock-free data structures for synchronization
- Thread pool for 20-thread utilization

### SIMD Optimization
- AVX2 intrinsics for 8-cell parallel evaluation
- Vectorized neighbor counting
- Aligned memory access

### Profiling & Benchmarking
- Measure frame times, simulation times separately
- Identify bottlenecks (CPU, GPU, memory bandwidth)
- Compare dense vs sparse grid performance
- Memory usage tracking

### Performance Targets
- 5-10M+ active cells @ 60 FPS
- Full 20-thread CPU utilization
- GPU compute offload working
- <2GB VRAM, <4GB RAM

---

## Phase 5: Multi-Scale Architecture

### Goals
Hierarchical grid structure supporting infinite zoom and meta-cellular automata.

### Quadtree Implementation
- Recursive spatial subdivision
- Nodes contain sub-grids or cells
- Lazy evaluation (only compute visited regions)
- Memory-efficient representation

### Level-of-Detail (LOD) System
- Coarse simulation at distance (simulate meta-cells)
- Fine simulation when zoomed in
- Seamless transitions between LOD levels
- Adaptive resolution based on zoom level

### Meta-Cells
- Cells that contain entire sub-grids
- Nested universes (GoL simulating GoL)
- Rendering hierarchical structures
- Zoom transitions between scales

### Infinite Zoom
- Progressive detail loading
- Fractal-like visualization
- Smooth camera zoom (log scale)
- Context preservation across scales

### Success Criteria
- Zoom from universe-scale to individual cells
- No performance degradation during zoom
- Seamless LOD transitions

---

## Phase 6: Pattern Library & Tools

### Goals
Rich collection of patterns and tools for exploration.

### Pattern Formats
- **RLE (Run Length Encoded)**: Standard format
- **Plaintext (.cells)**: Simple text format
- **Life 1.06**: Classic format
- Custom JSON format (optional)

### Pattern Library
- Curated collection organized by type:
  - Still lifes
  - Oscillators (by period)
  - Spaceships (by speed)
  - Guns (glider guns, other guns)
  - Methuselahs (long-lived chaotic patterns)
  - Turing machines
  - Meta-pixels (for GoL-in-GoL)

### Interactive Editor
- Mouse-based cell placement
- Pattern rotation (90°, 180°, 270°)
- Pattern copy/paste
- Selection tool
- Undo/redo
- Save/load custom patterns

### Pattern Analysis Tools
- Period detection (for oscillators)
- Bounding box calculation
- Population counting over time
- Trajectory tracking (for spaceships)
- Pattern classification

### UI Controls
- Pattern browser/selector
- Playback controls (play, pause, step, reset)
- Speed control (FPS slider)
- Generation counter
- Population statistics

---

## Phase 7: Rule 110 (1D Cellular Automaton)

### Goals
Implement and visualize Rule 110, demonstrating Turing completeness.

### Rule 110 Basics
- **1D cellular automaton**: Linear array of cells
- **Rule**: Based on cell and its 2 neighbors (3-cell window)
- **Rule 110 encoding**:
  ```
  111 110 101 100 011 010 001 000  (neighborhoods)
   0   1   1   0   1   1   1   0   (next state)
  ```
  Binary: 01101110 = 110 (decimal)
- **Turing complete**: Can simulate any computation

### Implementation
- `Rule110` class separate from GoL
- 1D array of cells with wrapping or fixed boundaries
- Update rule based on 3-cell neighborhoods
- Configurable rule number (support other rules: 30, 90, etc.)

### Visualization
- 2D scrolling display (time on Y-axis)
- Each row = one generation
- Scroll down as time progresses
- Color coding for cell states
- Side-by-side with GoL (optional)

### Example Programs
- Demonstrate Turing completeness:
  - Simple repeating patterns
  - Gliders in Rule 110
  - Universal Turing machine construction (complex)

### Success Criteria
- Rule 110 evolution matches reference implementations
- Visualize 1000+ generations
- Demonstrate interesting emergent behavior

---

## Phase 8: Self-Replication (Meta-GoL)

### Goals
Implement Game of Life simulating itself - the ultimate demonstration of emergence.

### Meta-Pixel Concepts
- **OTCA Metapixel**: Unit cell that simulates GoL
- **QTM (Quest for Tetris Metapixel)**: More efficient design
- Each meta-pixel is made of thousands of GoL cells
- Meta-pixels communicate via signals (gliders, etc.)

### Implementation Challenges
- **Scale**: Meta-patterns are enormous (10,000x10,000+ cells)
- **Performance**: Need all optimizations from Phase 5
- **Memory**: Sparse grid essential
- **Rendering**: Multi-scale required

### Hierarchical Simulation
- Outer GoL universe contains meta-pixels
- Each meta-pixel simulates an inner GoL universe
- Two levels of simulation running simultaneously
- Visualization shows both levels

### Visualization Strategy
- **Zoomed out**: See meta-pattern behavior (outer universe)
- **Zooming in**: Transition to see individual cells (inner universe)
- **Split view**: Show both scales simultaneously
- **Highlight**: Show which meta-pixels are "alive"

### Patterns to Implement
- Simple meta-pixel blinker
- Meta-pixel glider
- Full OTCA metapixel (if performance allows)

### Success Criteria
- Successfully demonstrate GoL-in-GoL
- Smooth visualization across scales
- Performance remains acceptable (>30 FPS)
- Clear demonstration of emergence

---

## Project Structure (Final)

```
game-of-life/
├── CMakeLists.txt
├── README.md
├── spec.md (this file)
├── .gitignore
├── include/
│   ├── core/
│   │   ├── Grid.h                    # Dense grid
│   │   ├── SparseGrid.h              # Sparse infinite grid
│   │   └── Cell.h                    # Cell state (optional)
│   ├── simulation/
│   │   ├── Simulator.h               # Main simulator
│   │   ├── Rule110.h                 # 1D CA engine
│   │   └── MetaSimulator.h           # Hierarchical simulation
│   ├── rendering/
│   │   ├── VulkanRenderer.h          # Vulkan abstraction
│   │   ├── Camera.h                  # 2D camera system
│   │   ├── Shader.h                  # Shader management
│   │   └── FrameBuffer.h             # Rendering targets
│   ├── patterns/
│   │   ├── PatternLoader.h           # RLE/Plaintext parser
│   │   ├── PatternLibrary.h          # Built-in patterns
│   │   └── PatternEditor.h           # Interactive editing
│   └── utils/
│       ├── Math.h                    # Vector/matrix utilities
│       ├── Profiler.h                # Performance profiling
│       └── Logger.h                  # Debug logging
├── src/
│   ├── main.cpp
│   ├── core/
│   ├── simulation/
│   ├── rendering/
│   ├── patterns/
│   └── utils/
├── shaders/
│   ├── cell.vert                     # Vertex shader
│   ├── cell.frag                     # Fragment shader
│   └── compile.sh                    # SPIR-V compilation
├── kernels/
│   └── gol.cu                        # CUDA kernels (optional)
├── tests/
│   ├── test_grid.cpp
│   ├── test_simulator.cpp
│   ├── test_patterns.cpp
│   └── test_rule110.cpp
├── patterns/
│   ├── still_lifes/
│   ├── oscillators/
│   ├── spaceships/
│   ├── guns/
│   └── metapixels/
├── docs/
│   ├── conway_rules.md
│   ├── rule110.md
│   ├── metapixels.md
│   └── performance.md
└── build/
```

---

## Design Principles

### Separation of Concerns
- **Grid**: Dumb data storage
- **Simulator**: Rule application logic
- **Renderer**: Visualization only
- **Patterns**: Data and I/O

### Performance First
- Cache-friendly data layouts
- Minimize memory allocations
- GPU acceleration where beneficial
- Profile before optimizing

### Extensibility
- Easy to add new CA rules
- Pluggable rendering backends
- Modular pattern system

### Testing
- Unit tests for core logic
- Integration tests for full pipeline
- Visual tests for rendering
- Benchmark tests for performance

---

## Learning Objectives

### Technical Skills
- Modern C++23 features
- Vulkan API and GPU programming
- Multi-threading and synchronization
- SIMD optimization (AVX2)
- Cache optimization techniques
- Software architecture patterns

### Domain Knowledge
- Cellular automata theory
- Emergence and complexity
- Turing completeness
- Self-replication in computational systems
- Moore vs Von Neumann neighborhoods

### Software Engineering
- Test-driven development
- Performance profiling
- Memory management
- API design
- Project organization

---

## References

### Conway's Game of Life
- [LifeWiki](https://conwaylife.com/wiki/) - Comprehensive pattern database
- [Golly](http://golly.sourceforge.net/) - Reference implementation

### Rule 110
- [Wolfram MathWorld](https://mathworld.wolfram.com/Rule110.html)
- [Elementary Cellular Automata](https://en.wikipedia.org/wiki/Elementary_cellular_automaton)

### Meta-Cellular Automata
- [OTCA Metapixel](https://conwaylife.com/wiki/OTCA_metapixel)
- [Quest for Tetris](https://codegolf.stackexchange.com/questions/11880/build-a-working-game-of-tetris-in-conways-game-of-life)

### Technical Resources
- [Vulkan Tutorial](https://vulkan-tutorial.com/)
- [C++23 Features](https://en.cppreference.com/w/cpp/23)
- [Intel Intrinsics Guide](https://www.intel.com/content/www/us/en/docs/intrinsics-guide/)
- [CUDA Programming Guide](https://docs.nvidia.com/cuda/cuda-c-programming-guide/)

---

## Current Status

**Phase 1**: COMPLETED (Foundation - Grid, Simulator, Conway's rules)

**Phase 2**: COMPLETED (Unit testing with Google Test)

**Phase 3**: IN PROGRESS (Vulkan visualization)

**Phase 4**: PENDING (Performance optimization)

**Phase 5**: PENDING (Multi-scale architecture)

**Phase 6**: PENDING (Pattern library & tools)

**Phase 7**: PENDING (Rule 110)

**Phase 8**: PENDING (Meta-GoL)
