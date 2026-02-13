# FPS Shooter - Project Summary

## Overview
A production-grade, modular 3D First-Person Shooter game built with C++17 and OpenGL 3.3 (DirectX 10 equivalent). Designed for cross-platform deployment including compilation to Windows EXE from GitHub Codespaces.

## File Structure

```
fps-game/
├── .github/workflows/     # CI/CD configuration
│   └── build.yml          # GitHub Actions build workflow
├── assets/                # Game assets (shaders, textures, models, sounds)
├── cmake/
│   └── Toolchain-mingw64.cmake  # MinGW cross-compilation toolchain
├── include/               # Header files
│   ├── core/              # Core systems (Window, Input, Timer, Logger)
│   ├── game/              # Game logic (Player, Weapon, Enemy, GameWorld)
│   ├── math/              # Math utilities (Vectors, Matrices, AABB, etc.)
│   └── renderer/          # Rendering system (Shader, Mesh, Camera, etc.)
├── scripts/               # Build scripts
│   ├── build_linux.sh     # Linux native build
│   ├── build_windows.sh   # Windows cross-compilation
│   └── fetch_deps.sh      # Dependency fetching
├── src/                   # Source files
│   ├── core/              # Core system implementations
│   ├── renderer/          # Renderer implementations
│   └── main.cpp           # Entry point
├── third_party/           # External dependencies
│   ├── glad/              # OpenGL loader
│   ├── stb/               # stb_image for texture loading
│   ├── glfw/              # Window/input (fetched during build)
│   └── glm/               # Math library (fetched during build)
├── .gitignore             # Git ignore rules
├── CMakeLists.txt         # Main CMake configuration
├── LICENSE                # MIT License
├── Makefile               # Convenience makefile
└── README.md              # Project documentation
```

## Key Features

### Core Systems
- **Window Management**: GLFW-based with fullscreen, vsync, and resize support
- **Input System**: Keyboard and mouse with raw input support
- **Timer**: High-precision frame timing with FPS calculation
- **Logger**: Thread-safe logging with file and console output

### Renderer
- **OpenGL 3.3 Core Profile**: Modern, efficient rendering
- **Shader System**: Compile-time and runtime shader loading
- **Mesh System**: Support for cubes, spheres, cylinders, and custom meshes
- **Texture System**: 2D textures and cubemaps with mipmapping
- **Camera**: Perspective/orthographic with frustum culling
- **Lighting**: Directional, point, and spot lights

### Game Systems
- **Player**: FPS controller with smooth movement and mouse look
- **Weapon System**: Shooting with projectiles and collision
- **Enemy AI**: State machine with chasing behavior
- **Wave System**: Progressive difficulty with enemy spawning
- **Collision**: Raycasting and bounding volume checks

## Build Instructions

### From GitHub Codespaces (Linux)
```bash
# Build Linux executable
make linux

# Build Windows executable (cross-compilation)
make windows
```

### From Windows (Visual Studio)
```bash
mkdir build && cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

## Controls
- **W/A/S/D**: Move
- **Mouse**: Look around
- **Left Click**: Shoot
- **Left Shift**: Sprint
- **Space**: Move up
- **Ctrl**: Move down
- **Escape**: Quit

## Technical Specifications

### Graphics
- OpenGL 3.3 Core Profile
- GLSL 3.30 shaders
- DirectX 10 equivalent hardware support
- Forward rendering with Phong lighting

### Performance Targets
- 144+ FPS on modern hardware
- 60+ FPS on DX10-era GPUs
- Efficient batching and state management

### Dependencies
- GLFW 3.3.8 (window/input)
- GLM 0.9.9.8 (math)
- GLAD (OpenGL loader)
- stb_image (texture loading)

## Production-Grade Features

1. **Modular Architecture**: Clean separation of concerns
2. **Error Handling**: Comprehensive error checking and logging
3. **Memory Management**: RAII principles, smart pointers
4. **Cross-Platform**: Linux, Windows (native and cross-compiled)
5. **CI/CD**: GitHub Actions for automated builds
6. **Documentation**: Comprehensive README and code comments

## Extensibility

The engine is designed for easy extension:
- Add new weapon types via `Weapon` class inheritance
- Create new enemy behaviors via `Enemy` state machine
- Implement new rendering effects via custom shaders
- Add audio via the stubbed `AudioManager`

## License
MIT License - See LICENSE file
