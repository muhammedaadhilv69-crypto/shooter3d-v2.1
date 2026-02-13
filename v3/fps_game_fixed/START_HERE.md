# FPS Game - Getting Started Index

## 🎉 NEW: Portable/Standalone Build Available!

**Want a truly portable game with zero installation?** 
See **PORTABLE_BUILD_GUIDE.md** for building a standalone version with all DLLs included!

```bash
# Quick portable build:
./download_sdl2.sh         # Download SDL2 source (one-time)
./build_portable.sh         # Build everything

# Result: build-portable/bin/ folder is 100% portable!
# Copy to any machine and run - no installation needed!
```

---

## 📖 Read These First

1. **QUICKSTART.md** - Start here! Platform-specific build commands
2. **COMPLETE_SUMMARY.md** - Full project overview and features
3. **README.md** - Project introduction and basic info
4. **BUILD_GUIDE.md** - Detailed build instructions for all platforms
5. **PROJECT_STRUCTURE.txt** - Code architecture and file organization

## 🚀 Quick Commands

### Linux/macOS - One Command Build
```bash
./build.sh && cd build/bin && ./FPSGame
```

### Windows (MSYS2) - One Command Build
```bash
mkdir build && cd build && cmake .. -G "MinGW Makefiles" && mingw32-make && cd bin && ./FPSGame.exe
```

### GitHub Codespaces - Build for Windows
```bash
sudo apt-get update && sudo apt-get install -y libsdl2-dev libgl1-mesa-dev cmake build-essential mingw-w64
mkdir build-windows && cd build-windows
cmake .. -DCMAKE_TOOLCHAIN_FILE=../toolchain-mingw.cmake -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
# Download: build-windows/bin/FPSGame.exe
```

## 📁 Project Structure

```
fps_game/
│
├── 📄 Documentation
│   ├── QUICKSTART.md           ← START HERE
│   ├── COMPLETE_SUMMARY.md     ← Full overview
│   ├── README.md               ← Project intro
│   ├── BUILD_GUIDE.md          ← Detailed build help
│   └── PROJECT_STRUCTURE.txt   ← Architecture
│
├── 🔨 Build Files
│   ├── CMakeLists.txt          ← Main build config
│   ├── build.sh                ← Linux/macOS build
│   ├── build_windows.bat       ← Windows build
│   ├── install_deps.sh         ← Install dependencies
│   └── toolchain-mingw.cmake   ← Cross-compile support
│
├── 💻 Source Code
│   └── src/
│       ├── main.cpp            ← Entry point
│       ├── engine/             ← Core engine (6 modules)
│       │   ├── Window          ← SDL2 window/input
│       │   ├── Renderer        ← OpenGL rendering
│       │   ├── Shader          ← GLSL shaders
│       │   ├── Camera          ← FPS camera
│       │   ├── Mesh            ← 3D geometry
│       │   └── Texture         ← Texture loading
│       └── game/               ← Game logic (6 modules)
│           ├── Game            ← Main game loop
│           ├── Player          ← Player controller
│           ├── Weapon          ← Weapon system
│           ├── Enemy           ← Enemy AI
│           ├── Level           ← Level generation
│           └── Particle        ← Particle effects
│
└── 📚 Dependencies
    └── external/glm/           ← Math library (included)
```

## 🎮 What You Get

✅ **Full 3D FPS Game** - Complete and playable
✅ **Production Code** - Clean, modular C++17
✅ **Cross-Platform** - Linux, Windows, macOS
✅ **No Assets Needed** - All procedurally generated
✅ **Modern OpenGL** - OpenGL 3.3 Core Profile
✅ **Game Features**:
   - Wave-based survival gameplay
   - Weapon system with ammo and reload
   - Enemy AI with detection and pursuit
   - Particle effects
   - Score tracking
   - Procedural arena level

## 🎯 Controls

| Input | Action |
|-------|--------|
| W, A, S, D | Move |
| Mouse | Look around |
| Left Click | Shoot |
| R | Reload |
| Space | Jump |
| ESC | Quit |

## 🔧 System Requirements

**Minimum:**
- OpenGL 3.3 capable GPU
- 512MB RAM
- Dual-core CPU

**Recommended:**
- Dedicated GPU
- 1GB RAM
- Quad-core CPU

## 📦 Dependencies

**Required (must install):**
- CMake 3.10+
- C++17 compiler
- SDL2 development libraries
- OpenGL 3.3+ support

**Included (no installation needed):**
- GLM math library (header-only)

## 🐛 Troubleshooting

**Build fails - SDL2 not found:**
```bash
# Ubuntu/Debian
sudo apt-get install libsdl2-dev

# Fedora/RHEL
sudo dnf install SDL2-devel

# macOS
brew install sdl2

# Windows (MSYS2)
pacman -S mingw-w64-x86_64-SDL2
```

**Build fails - OpenGL not found:**
```bash
# Ubuntu/Debian
sudo apt-get install libgl1-mesa-dev

# Other platforms: Update GPU drivers
```

**Game builds but won't run:**
- Check OpenGL version: Your GPU must support OpenGL 3.3+
- Update GPU drivers
- Make sure you have a display (won't work in headless mode)

## 💡 Next Steps

1. **Read QUICKSTART.md** for your platform
2. **Install dependencies** (SDL2, CMake, compiler)
3. **Run build script** (./build.sh or build_windows.bat)
4. **Play the game!** (build/bin/FPSGame)

## 📝 Notes

- **Codespaces Users**: Can build but can't run (no display). Build for Windows and download the .exe
- **All platforms**: Make sure SDL2 and OpenGL are installed before building
- **No internet needed** for build after dependencies are installed
- **All game assets** are procedurally generated - no external files needed

## 🏆 Achievement Unlocked

You now have a complete, production-grade 3D FPS game with:
- ✅ 25 source files (12 headers + 12 implementations + main.cpp)
- ✅ Modular engine architecture
- ✅ Full game mechanics
- ✅ Cross-platform support
- ✅ Professional code quality
- ✅ Zero asset dependencies

**Ready to compile and play!**

---

For questions or issues, check BUILD_GUIDE.md for comprehensive troubleshooting.
