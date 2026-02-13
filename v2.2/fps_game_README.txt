╔══════════════════════════════════════════════════════════════════╗
║          FPS Game - Portable Build Edition v1.0                  ║
║          Production-Grade 3D First Person Shooter                ║
╚══════════════════════════════════════════════════════════════════╝

WHAT'S IN THIS ZIP
==================

✓ Complete C++ source code (25 files)
✓ Production-grade modular architecture
✓ OpenGL 3.3 renderer with Phong lighting
✓ Full FPS game mechanics (weapons, enemies, AI, particles)
✓ Portable build system (includes SDL2.dll)
✓ Cross-platform support (Windows, Linux, macOS)
✓ Comprehensive documentation (13 guides)

FILE SIZE: 56 KB (compressed)
EXTRACTED: ~200 KB (without SDL2 source)
WITH SDL2: ~6 MB (SDL2 source downloaded separately)
FINAL BUILD: ~2-3 MB portable game

QUICK START
===========

1. EXTRACT THE ZIP
   Extract fps_game_portable_v1.0.zip to any folder

2. CHOOSE YOUR PLATFORM:

   WINDOWS (MSYS2):
   ----------------
   - Open MSYS2 MinGW 64-bit terminal
   - cd to extracted folder
   - Run: chmod +x *.sh (if needed)
   - Run: ./download_sdl2.sh
   - Run: ./build_portable.sh
   
   OR double-click:
   - download_sdl2_windows.bat
   - build_portable_windows.bat

   LINUX/MAC:
   ----------
   - Open terminal
   - cd to extracted folder
   - Run: chmod +x *.sh
   - Run: ./download_sdl2.sh
   - Run: ./build_portable.sh

3. FIND YOUR GAME
   Location: build-portable/bin/
   - Contains: FPSGame.exe + SDL2.dll (Windows)
   - Or: FPSGame + libSDL2.so (Linux)

4. PLAY!
   Windows: Double-click FPSGame.exe
   Linux/Mac: ./FPSGame

5. DISTRIBUTE
   Zip the entire build-portable/bin/ folder
   Share it - runs anywhere with no installation!

CONTROLS
========

W, A, S, D    - Move
Mouse         - Look around  
Left Click    - Shoot
R             - Reload
Space         - Jump
ESC           - Quit

REQUIREMENTS
============

To Build:
- CMake 3.10+
- C++17 compiler (GCC/Clang/MSVC)
- Internet (to download SDL2 source once)

To Run Built Game:
- OpenGL 3.3+ capable GPU
- NO other dependencies!

DOCUMENTATION
=============

START HERE:
- INSTALL_NOTES.txt      - Post-extraction setup
- START_HERE.md          - Main navigation guide
- WHATS_NEW.txt          - What's new in portable build

QUICK GUIDES:
- QUICKSTART.md          - Platform-specific quick start
- PORTABLE_QUICK_REF.txt - Portable build reference card

DETAILED GUIDES:
- PORTABLE_BUILD_GUIDE.md - Complete portable build guide
- BUILD_GUIDE.md          - Detailed build instructions
- COMPLETE_SUMMARY_V2.md  - Full project overview

TECHNICAL:
- PROJECT_STRUCTURE.txt   - Code architecture
- FILE_MANIFEST.txt       - Complete file listing
- README.md               - Project introduction

FEATURES
========

✓ Wave-based survival gameplay
✓ Enemy AI with detection and pathfinding
✓ Weapon system (ammo, reload, fire rate)
✓ Particle effects
✓ Procedural arena level
✓ Score tracking
✓ Health system
✓ 3D graphics with lighting
✓ Smooth FPS controls

PORTABLE BUILD
==============

This version builds SDL2 from source and bundles it with the game.
Result: 100% standalone executable requiring ZERO installation!

Perfect for:
- USB distribution
- Game jams
- Beta testing
- Locked-down PCs
- Quick sharing

WHAT HAPPENS WHEN YOU BUILD:
1. SDL2 source downloaded (~6 MB, one-time)
2. SDL2 compiled from source (~5-10 min first time)
3. Game compiled and linked (~10-30 sec)
4. SDL2.dll auto-copied to bin/ folder
5. Result: Portable bin/ folder (~2-3 MB)

CROSS-COMPILATION
=================

Build Windows .exe from Linux (GitHub Codespaces):

sudo apt-get install cmake build-essential mingw-w64
./download_sdl2.sh
mkdir build-win && cd build-win
x86_64-w64-mingw32-cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

Download build-win/bin/ folder for Windows!

TROUBLESHOOTING
===============

Q: "Permission denied" on Linux/Mac
A: Run: chmod +x *.sh

Q: "SDL2 source not found"
A: Run: ./download_sdl2.sh or download_sdl2_windows.bat

Q: "CMake can't find compiler"
A: Install MinGW (Windows) or build-essential (Linux)

Q: Game won't run on another PC
A: Target needs OpenGL 3.3+ (update GPU drivers)

TECHNICAL SPECS
===============

Language:     C++17
Graphics:     OpenGL 3.3 Core Profile
Window/Input: SDL2 2.28.5
Math:         GLM (custom implementation)
Build:        CMake 3.10+
Platforms:    Windows 10+, Linux, macOS 10.15+
Lines of Code: ~3,500

CODE QUALITY
============

✓ Modern C++17 practices
✓ RAII resource management
✓ Smart pointers (no memory leaks)
✓ Modular architecture
✓ Production-grade structure
✓ Well-commented
✓ Cross-platform

BUGS FIXED
==========

✓ Added <algorithm> header for std::min/max
✓ Added M_PI definition for portability
✓ All memory properly managed with RAII
✓ No memory leaks
✓ Consistent include paths
✓ Portable across compilers

LICENSE
=======

MIT License - Free for any use, commercial or personal.

CREDITS
=======

Built from scratch using:
- C++17 Standard Library
- OpenGL 3.3 Core Profile
- SDL2 2.28.5
- Custom GLM implementation

No game engines or frameworks used.

SUPPORT
=======

For detailed help, see the documentation files.
All common issues are covered in:
- PORTABLE_BUILD_GUIDE.md
- BUILD_GUIDE.md
- QUICKSTART.md

Have fun! 🎮

═══════════════════════════════════════════════════════════════════

Version: 1.0 Portable Edition
Date: February 2026
Package: fps_game_portable_v1.0.zip

Ready to build, ready to play, ready to share! 🚀

═══════════════════════════════════════════════════════════════════
