# Portable Build Guide - Standalone FPS Game

This guide shows you how to build a **completely portable/standalone version** of the FPS game that includes all DLLs and requires **zero installation** on the target machine.

## What is Portable Mode?

In portable mode:
- ✅ SDL2 is compiled from source and bundled as SDL2.dll (Windows) or libSDL2.so (Linux)
- ✅ All dependencies are included in the bin/ folder
- ✅ No installation required on target machine
- ✅ Just copy the bin/ folder and run!
- ✅ OpenGL drivers are the only system requirement (already on Windows/Linux)

## Quick Start

### Windows (MSYS2/MinGW)

```batch
# 1. Download SDL2 source
download_sdl2_windows.bat

# 2. Build everything
build_portable_windows.bat

# 3. Your portable game is in: build-portable\bin\
# Copy the entire folder to any Windows PC!
```

### Linux/macOS

```bash
# 1. Download SDL2 source (automatic)
./download_sdl2.sh

# 2. Build everything
./build_portable.sh

# 3. Your portable game is in: build-portable/bin/
# Copy the entire folder to any compatible system!
```

### GitHub Codespaces (Build Windows Portable .exe)

```bash
# Install minimal dependencies (just compiler, no SDL2!)
sudo apt-get update
sudo apt-get install -y cmake build-essential mingw-w64

# Download SDL2 source
./download_sdl2.sh

# Build Windows portable version
mkdir build-windows-portable
cd build-windows-portable

# Configure for Windows with SDL2 built from source
x86_64-w64-mingw32-cmake .. -DCMAKE_BUILD_TYPE=Release

# Build (compiles SDL2 + game)
make -j$(nproc)

# Download build-windows-portable/bin/ folder
# Contains: FPSGame.exe + SDL2.dll + everything needed!
```

## Detailed Instructions

### Windows - Step by Step

#### Requirements
- CMake 3.10+
- MinGW-w64 compiler
- Internet connection (to download SDL2 source once)

#### Option 1: MSYS2 (Recommended)

1. **Install MSYS2** from https://www.msys2.org/

2. **Open MSYS2 MinGW 64-bit terminal** and install build tools:
```bash
pacman -Syu
pacman -S mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc make tar
```

3. **Navigate to game folder**:
```bash
cd /path/to/fps_game
```

4. **Download SDL2 source** (one-time):
```bash
./download_sdl2.sh
# Or on Windows: download_sdl2_windows.bat
```

5. **Build portable version**:
```bash
./build_portable.sh
# Or on Windows: build_portable_windows.bat
```

6. **Find your portable game**:
```
build-portable/bin/
├── FPSGame.exe    ← Your game
└── SDL2.dll       ← Auto-included SDL2 library
```

7. **Copy the entire bin/ folder** to any Windows PC and run FPSGame.exe!

#### Option 2: Visual Studio

1. **Install Visual Studio 2019+** with C++ Desktop Development

2. **Open "Developer Command Prompt for VS"**

3. **Download SDL2 source**:
```cmd
download_sdl2_windows.bat
```

4. **Build**:
```cmd
mkdir build-portable
cd build-portable
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
```

5. **Your portable game**: `build-portable/bin/Release/`

### Linux - Step by Step

#### Requirements
- CMake 3.10+
- GCC or Clang
- Build essentials
- Internet connection (to download SDL2 source once)

#### Build Steps

1. **Install build tools** (no SDL2 needed!):
```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install -y cmake build-essential

# Fedora/RHEL
sudo dnf install -y cmake gcc-c++ make

# Arch
sudo pacman -S --needed cmake base-devel
```

2. **Download SDL2 source** (one-time):
```bash
./download_sdl2.sh
```

3. **Build portable version**:
```bash
./build_portable.sh
```

4. **Find your portable game**:
```
build-portable/bin/
├── FPSGame          ← Your game
└── libSDL2-2.0.so.0 ← Auto-included SDL2 library
```

5. **Copy the entire bin/ folder** to any Linux machine and run!

### macOS - Step by Step

Same as Linux, but install Xcode Command Line Tools first:
```bash
xcode-select --install
```

Then follow Linux instructions above.

## What Gets Built?

After building, your `build-portable/bin/` folder contains:

### Windows
```
bin/
├── FPSGame.exe        ← Game executable
└── SDL2.dll           ← SDL2 library (auto-copied)
```

### Linux
```
bin/
├── FPSGame            ← Game executable  
└── libSDL2-2.0.so.0   ← SDL2 library (auto-copied)
```

### macOS
```
bin/
├── FPSGame              ← Game executable
└── libSDL2-2.0.dylib    ← SDL2 library (auto-copied)
```

## Cross-Compilation (Advanced)

### Build Windows .exe from Linux (Codespaces)

```bash
# Install cross-compiler
sudo apt-get install -y mingw-w64 cmake

# Download SDL2 source
./download_sdl2.sh

# Create cross-compile build
mkdir build-windows-portable
cd build-windows-portable

# Configure for Windows
x86_64-w64-mingw32-cmake .. \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_SYSTEM_NAME=Windows

# Build
make -j$(nproc)

# Result: bin/FPSGame.exe + bin/SDL2.dll
```

Download the `bin/` folder and run on any Windows PC!

## Testing Portability

### Windows
1. Copy `build-portable/bin/` to a USB drive
2. Plug into another Windows PC (no admin rights needed)
3. Double-click `FPSGame.exe` - it just works!

### Linux
1. Copy `build-portable/bin/` to another Linux machine
2. Make executable: `chmod +x FPSGame`
3. Run: `./FPSGame`

## Build Times

- **First build**: 5-10 minutes (compiles SDL2 from source)
- **Subsequent builds**: 10-30 seconds (SDL2 already compiled)
- **Clean rebuild**: Use `./build_portable.sh` again

## Size

- **Windows**: ~2-3 MB (FPSGame.exe + SDL2.dll)
- **Linux**: ~2-3 MB (FPSGame + libSDL2.so)
- **No external dependencies!**

## Troubleshooting

**"SDL2 source not found"**
- Run `./download_sdl2.sh` or `download_sdl2_windows.bat`
- Or manually download from: https://github.com/libsdl-org/SDL/releases/
- Extract to `external/SDL2/`

**"CMake can't find compiler"**
- Windows: Install MinGW-w64 via MSYS2
- Linux: `sudo apt-get install build-essential`

**SDL2.dll not copied on Windows**
- This is automatic - check `build-portable/bin/`
- If missing, manually copy from `build-portable/bin/` after build

**Game won't run on target machine**
- Ensure target has OpenGL 3.3+ support (built into Windows/Linux)
- Update graphics drivers if needed
- No other dependencies required!

## Advantages Over System SDL2

| Portable Build | System SDL2 |
|----------------|-------------|
| ✅ No installation needed | ❌ Must install SDL2 |
| ✅ Copy and run anywhere | ❌ Each machine needs setup |
| ✅ Version controlled | ❌ Version conflicts |
| ✅ Truly standalone | ❌ Dependency hell |
| ✅ Works on locked-down PCs | ❌ Needs admin rights |

## Distribution

To distribute your game:

1. Build portable version
2. Zip the entire `build-portable/bin/` folder
3. Share the zip file
4. Users just extract and run - no installation!

Perfect for:
- Game jams
- Demos
- Beta testing
- LAN parties
- USB thumb drives
- Locked corporate PCs

## Next Steps

After building:
- Test on another machine to verify portability
- Create a README.txt in bin/ folder with controls
- Consider adding an icon to FPSGame.exe (Windows)
- Zip the bin/ folder for easy distribution

---

**You now have a truly portable, standalone game!**
No installation, no dependencies, just copy and play! 🎮
