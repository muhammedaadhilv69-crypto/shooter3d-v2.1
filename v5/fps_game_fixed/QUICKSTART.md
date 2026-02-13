# QUICK START GUIDE

## For GitHub Codespaces / Linux Environment

### 1. Install Dependencies
```bash
sudo apt-get update
sudo apt-get install -y libsdl2-dev libgl1-mesa-dev cmake build-essential
```

### 2. Build the Game
```bash
cd fps_game
./build.sh
```

### 3. Run (if you have a display)
```bash
cd build/bin
./FPSGame
```

### 4. For Codespaces - Build Windows EXE
Since Codespaces doesn't have a display, build a Windows executable to download:

```bash
# Install MinGW cross-compiler
sudo apt-get install -y mingw-w64

# Create build directory
mkdir build-windows
cd build-windows

# Configure for Windows
cmake .. \
  -DCMAKE_TOOLCHAIN_FILE=../toolchain-mingw.cmake \
  -DCMAKE_BUILD_TYPE=Release

# Build
make -j$(nproc)

# The Windows executable will be in: build-windows/bin/FPSGame.exe
```

## For Windows (Local Machine)

### Option A: MSYS2 (Recommended)
1. Download and install MSYS2 from https://www.msys2.org/
2. Open "MSYS2 MinGW 64-bit" terminal
3. Run:
```bash
pacman -Syu
pacman -S mingw-w64-x86_64-SDL2 mingw-w64-x86_64-cmake mingw-w64-x86_64-gcc make

cd fps_game
mkdir build && cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Release
mingw32-make -j4

cd bin
./FPSGame.exe
```

### Option B: Visual Studio
1. Install Visual Studio 2019+ with C++ Desktop Development
2. Download SDL2 from https://www.libsdl.org/download-2.0.php (VC development libraries)
3. Extract SDL2 somewhere (e.g., C:\SDL2)
4. Run:
```cmd
cd fps_game
mkdir build
cd build
cmake .. -DSDL2_DIR="C:\SDL2"
cmake --build . --config Release

copy C:\SDL2\lib\x64\SDL2.dll bin\Release\
cd bin\Release
FPSGame.exe
```

## For Linux (Ubuntu/Debian)

```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y libsdl2-dev libgl1-mesa-dev cmake build-essential

# Build
cd fps_game
./build.sh

# Run
cd build/bin
./FPSGame
```

## For macOS

```bash
# Install Homebrew if needed: /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

# Install dependencies
brew install sdl2 cmake

# Build
cd fps_game
./build.sh

# Run
cd build/bin
./FPSGame
```

## Controls
- **WASD** - Move
- **Mouse** - Look around
- **Left Click** - Shoot
- **R** - Reload
- **Space** - Jump
- **ESC** - Quit

## Game Objective
Survive waves of enemies in an arena. Each wave spawns more enemies.
Enemies turn red when they detect you. Shoot them before they reach you!

## Troubleshooting

**"SDL2 not found"**
- Make sure SDL2 is installed: `pkg-config --modversion sdl2`
- On Windows with MSYS2: `pacman -S mingw-w64-x86_64-SDL2`

**"OpenGL not found"**
- Install Mesa: `sudo apt-get install libgl1-mesa-dev` (Linux)
- On Windows, OpenGL should come with GPU drivers

**Game won't start**
- Make sure you have OpenGL 3.3+ support
- Update your GPU drivers
- Check if a display is available (won't work in headless environments)

**Black screen**
- Your GPU might not support OpenGL 3.3
- Try updating graphics drivers

## Development

See BUILD_GUIDE.md for detailed build options and troubleshooting.
See PROJECT_STRUCTURE.txt for code architecture.
See README.md for feature overview.
