# FPS Shooter - OpenGL 3.3

A production-grade, lightweight 3D First-Person Shooter game built with C++ and OpenGL 3.3 (DirectX 10 equivalent).

## Features

- **Modern C++17** codebase with modular architecture
- **OpenGL 3.3** rendering (backward compatible with DX10-era hardware)
- **Cross-platform**: Builds on Linux and Windows (via MinGW cross-compilation)
- **Lightweight**: Minimal dependencies, optimized for performance
- **Production-grade code**: Clean architecture, proper error handling, memory management

### Game Features

- FPS camera with mouse look and WASD movement
- Weapon system with shooting and projectiles
- Enemy AI with chasing behavior
- Wave-based spawning system
- Collision detection
- Lighting (ambient, diffuse, specular)
- Debug rendering

## Project Structure

```
fps-game/
├── src/
│   ├── core/          # Window, Input, Timer, Logger
│   ├── renderer/      # Shader, Mesh, Texture, Camera, Renderer
│   ├── game/          # Player, Weapon, Enemy, GameWorld
│   ├── math/          # Math utilities
│   └── main.cpp       # Entry point
├── include/           # Header files
├── third_party/       # External dependencies
├── assets/            # Shaders, textures, models
├── scripts/           # Build scripts
└── cmake/             # CMake toolchain files
```

## Building

### Prerequisites

- CMake 3.16+
- C++17 compiler (GCC, Clang, or MSVC)
- Git

### Linux (Native)

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get update
sudo apt-get install -y build-essential cmake git libgl1-mesa-dev

# Build
bash scripts/build_linux.sh

# Run
./build/linux/FPSShooter
```

### Windows (Cross-compilation from Linux/Codespaces)

```bash
# Build Windows executable
bash scripts/build_windows.sh

# The executable will be at:
# ./build/windows/FPSShooter.exe
```

### Windows (Native with Visual Studio)

```bash
mkdir build
cd build
cmake .. -G "Visual Studio 17 2022"
cmake --build . --config Release
```

## Controls

| Key | Action |
|-----|--------|
| W/A/S/D | Move |
| Mouse | Look around |
| Left Click | Shoot |
| Left Shift | Sprint |
| Space | Move up |
| Ctrl | Move down |
| Escape | Quit |

## Architecture

### Core Systems

- **Window**: GLFW-based window management
- **Input**: Keyboard and mouse input handling
- **Timer**: Frame timing and delta time calculation
- **Logger**: Thread-safe logging system

### Renderer

- **Shader**: GLSL shader compilation and uniform management
- **Mesh**: Vertex array/buffer management with support for multiple primitives
- **Texture**: 2D texture and cubemap loading
- **Camera**: Perspective/orthographic camera with frustum culling
- **Renderer**: Main rendering system with batching and state management

### Game

- **Player**: FPS controller with camera and movement
- **Weapon**: Shooting mechanics with projectiles
- **Enemy**: AI-controlled enemies with chasing behavior
- **GameWorld**: Main game loop and state management

## Performance

- Target: 144+ FPS on modern hardware
- Optimized for OpenGL 3.3 / DirectX 10 equivalent GPUs
- Efficient batching and minimal state changes
- Frustum culling for visibility optimization

## License

MIT License - See LICENSE file for details

## Credits

- GLFW: Window and input management
- GLAD: OpenGL function loading
- GLM: Mathematics library
- stb_image: Image loading
