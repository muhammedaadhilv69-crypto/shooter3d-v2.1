# FPS Game - Production-Grade 3D Shooter

A modular, lightweight 3D first-person shooter built with C++ and OpenGL 3.3.

## 🚀 NEW: Fully Portable/Standalone Build!

**No installation required!** The game now builds SDL2 from source and bundles it as a DLL/SO, making it completely portable. Just build once, copy the `bin/` folder, and run on any machine!

See **PORTABLE_BUILD_GUIDE.md** for complete instructions.

### Quick Portable Build:
```bash
# Linux/macOS
./download_sdl2.sh
./build_portable.sh

# Windows
download_sdl2_windows.bat
build_portable_windows.bat

# Result: build-portable/bin/ contains everything!
# Copy to any machine and run - no dependencies needed!
```

## Features

- **Full 3D FPS mechanics** - WASD movement, mouse look, jumping
- **Weapon system** - Rifle with realistic reload mechanics
- **Enemy AI** - Enemies track and attack the player
- **Particle effects** - Visual feedback for hits and kills
- **Wave-based gameplay** - Increasing difficulty
- **Procedural level** - Arena-style map with obstacles
- **Production-quality code** - Modular architecture, clean separation of concerns

## Controls

- **WASD** - Move
- **Mouse** - Look around
- **Left Click** - Shoot
- **R** - Reload
- **Space** - Jump
- **ESC** - Quit

## Building

### Requirements
- CMake 3.10+
- C++17 compiler (GCC/Clang/MSVC)
- SDL2
- OpenGL 3.3+ support

### Linux/macOS
```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install libsdl2-dev cmake build-essential

# Build
./build.sh

# Run
cd build/bin
./FPSGame
```

### Windows (MinGW)
```bash
# Install dependencies via MSYS2
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-cmake

# Build
mkdir build && cd build
cmake .. -G "MinGW Makefiles"
mingw32-make

# Run
cd bin
FPSGame.exe
```

## Architecture

```
src/
├── engine/          # Core engine systems
│   ├── Window      # Window management (SDL2)
│   ├── Renderer    # OpenGL rendering
│   ├── Shader      # Shader compilation/management
│   ├── Camera      # FPS camera
│   ├── Mesh        # 3D mesh primitives
│   └── Texture     # Texture loading
├── game/           # Game-specific code
│   ├── Player      # Player controller
│   ├── Weapon      # Weapon system
│   ├── Enemy       # Enemy AI
│   ├── Level       # Level generation
│   ├── Particle    # Particle effects
│   └── Game        # Main game loop
└── main.cpp        # Entry point
```

## Technical Details

- **Graphics API**: OpenGL 3.3 Core Profile
- **Math Library**: GLM (header-only)
- **Window/Input**: SDL2
- **Rendering**: Forward rendering with Phong lighting
- **Geometry**: Procedurally generated primitives (cubes, spheres, planes)

## License

MIT License - Feel free to use for learning or commercial projects
