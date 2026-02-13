# Build Guide - FPS Game

This guide covers building the FPS game on different platforms.

## Quick Start (Linux with SDL2 installed)

```bash
./build.sh
cd build/bin
./FPSGame
```

## Platform-Specific Instructions

### Ubuntu/Debian Linux

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y libsdl2-dev libgl1-mesa-dev cmake build-essential

# Build
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Run
./bin/FPSGame
```

### Fedora/RHEL/CentOS

```bash
# Install dependencies
sudo dnf install -y SDL2-devel mesa-libGL-devel cmake gcc-c++

# Build
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Run
./bin/FPSGame
```

### Arch Linux/Manjaro

```bash
# Install dependencies
sudo pacman -S --needed sdl2 mesa cmake base-devel

# Build
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# Run
./bin/FPSGame
```

### macOS

```bash
# Install dependencies (requires Homebrew)
brew install sdl2 cmake

# Build
mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(sysctl -n hw.ncpu)

# Run
./bin/FPSGame
```

### Windows (MSYS2/MinGW)

1. Install MSYS2 from https://www.msys2.org/
2. Open MSYS2 MinGW 64-bit terminal
3. Install dependencies:

```bash
pacman -Syu
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc make
```

4. Build:

```bash
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
mingw32-make -j4
```

5. Run:

```bash
cd bin
./FPSGame.exe
```

### Windows (Visual Studio)

1. Install Visual Studio 2019 or later with C++ support
2. Download SDL2 development libraries from https://www.libsdl.org/
3. Extract SDL2 to a known location
4. Build:

```cmd
mkdir build
cd build
cmake .. -DSDL2_DIR="C:\path\to\SDL2"
cmake --build . --config Release
```

5. Copy SDL2.dll to build\bin\Release\
6. Run FPSGame.exe

## GitHub Codespaces Build

If building in GitHub Codespaces (headless environment):

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y libsdl2-dev libgl1-mesa-dev cmake build-essential

# Build for cross-compilation to Windows (optional)
sudo apt-get install -y mingw-w64

# Build Linux version
./build.sh

# Cross-compile for Windows
mkdir build-windows && cd build-windows
x86_64-w64-mingw32-cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
```

Note: The game requires a display. In Codespaces, build completes but execution requires X11 forwarding or downloading the binary to run locally.

## Troubleshooting

### SDL2 not found

```bash
# Check if SDL2 is installed
pkg-config --modversion sdl2

# If not installed, use platform-specific install command above
```

### OpenGL not found

```bash
# Ubuntu/Debian
sudo apt-get install libgl1-mesa-dev

# Fedora
sudo dnf install mesa-libGL-devel

# Arch
sudo pacman -S mesa
```

### CMake version too old

```bash
# Install newer CMake from pip
pip3 install --user cmake
# Or download from https://cmake.org/download/
```

## Build Options

```bash
# Debug build
cmake .. -DCMAKE_BUILD_TYPE=Debug

# Release with debug info
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo

# Specify compiler
cmake .. -DCMAKE_C_COMPILER=gcc-11 -DCMAKE_CXX_COMPILER=g++-11
```

## Post-Build

After successful build:
- Executable location: `build/bin/FPSGame` (or `FPSGame.exe` on Windows)
- No external resources needed - all game data is procedurally generated
- No configuration files required

## Performance

- Recommended: Dedicated GPU with OpenGL 3.3+ support
- Minimum: Integrated graphics with OpenGL 3.3 support
- RAM: 512MB minimum, 1GB recommended
- CPU: Any dual-core processor from the last 10 years
