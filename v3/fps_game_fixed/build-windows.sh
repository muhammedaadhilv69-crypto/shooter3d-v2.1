#!/usr/bin/env bash
# =============================================================================
# build-windows.sh — Cross-compile FPS Game for Windows from Linux/macOS
# Produces:  windows-build/FPSGame.exe + SDL2.dll, zipped as FPSGame-Windows.zip
#
# Requirements (Ubuntu/Debian):
#   sudo apt-get install cmake build-essential mingw-w64
# =============================================================================
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

BUILD_DIR="$SCRIPT_DIR/windows-build-tmp"
OUT_DIR="$SCRIPT_DIR/windows-build"
ZIP_NAME="FPSGame-Windows.zip"

# --------------------------------------------------------------------------
# 1. Check dependencies
# --------------------------------------------------------------------------
echo "=== Checking build dependencies ==="
for tool in cmake x86_64-w64-mingw32-g++ x86_64-w64-mingw32-gcc zip; do
    if ! command -v "$tool" &>/dev/null; then
        echo ""
        echo "ERROR: '$tool' not found."
        echo ""
        echo "Install on Ubuntu/Debian:"
        echo "  sudo apt-get install cmake build-essential mingw-w64 zip"
        echo ""
        echo "Install on Fedora/RHEL:"
        echo "  sudo dnf install cmake gcc-c++ mingw64-gcc mingw64-gcc-c++ zip"
        echo ""
        echo "Install on macOS (Homebrew):"
        echo "  brew install cmake mingw-w64 zip"
        exit 1
    fi
done
echo "All dependencies found!"

# --------------------------------------------------------------------------
# 2. Download SDL2 source if not present
# --------------------------------------------------------------------------
SDL2_VERSION="2.28.5"
SDL2_URL="https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/SDL2-${SDL2_VERSION}.tar.gz"
SDL2_DIR="$SCRIPT_DIR/external/SDL2"

if [ ! -d "$SDL2_DIR" ]; then
    echo ""
    echo "=== Downloading SDL2 ${SDL2_VERSION} source ==="
    mkdir -p "$SCRIPT_DIR/external"
    cd "$SCRIPT_DIR/external"

    if command -v curl &>/dev/null; then
        curl -L "$SDL2_URL" -o SDL2.tar.gz
    elif command -v wget &>/dev/null; then
        wget -q --show-progress "$SDL2_URL" -O SDL2.tar.gz
    else
        echo "ERROR: Neither curl nor wget found. Please install one."
        exit 1
    fi

    echo "Extracting SDL2..."
    tar -xzf SDL2.tar.gz
    mv "SDL2-${SDL2_VERSION}" SDL2
    rm SDL2.tar.gz
    cd "$SCRIPT_DIR"
    echo "SDL2 ready!"
else
    echo "SDL2 source already present at external/SDL2"
fi

# --------------------------------------------------------------------------
# 3. Write MinGW toolchain file
# --------------------------------------------------------------------------
echo ""
echo "=== Configuring MinGW cross-compile toolchain ==="
TOOLCHAIN_FILE="$SCRIPT_DIR/toolchain-mingw64.cmake"
cat > "$TOOLCHAIN_FILE" << 'CMAKE_TC'
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_PROCESSOR x86_64)

# Cross-compiler executables
set(CMAKE_C_COMPILER   x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER  x86_64-w64-mingw32-windres)

# Target environment paths
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
CMAKE_TC

echo "Toolchain file written."

# --------------------------------------------------------------------------
# 4. CMake configure
# --------------------------------------------------------------------------
echo ""
echo "=== Configuring CMake for Windows (MinGW) ==="
rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"

cmake -S "$SCRIPT_DIR" -B "$BUILD_DIR" \
    -DCMAKE_TOOLCHAIN_FILE="$TOOLCHAIN_FILE" \
    -DCMAKE_BUILD_TYPE=Release \
    -DSDL_SHARED=ON \
    -DSDL_STATIC=OFF \
    -DSDL_TEST=OFF \
    -DCMAKE_EXE_LINKER_FLAGS="-static-libgcc -static-libstdc++" \
    2>&1

echo "CMake configure done."

# --------------------------------------------------------------------------
# 5. Build
# --------------------------------------------------------------------------
echo ""
echo "=== Building (this may take 5-15 minutes for first SDL2 compile) ==="
CORES=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)
cmake --build "$BUILD_DIR" --config Release -j"$CORES" 2>&1
echo "Build complete!"

# --------------------------------------------------------------------------
# 6. Collect outputs
# --------------------------------------------------------------------------
echo ""
echo "=== Collecting output files ==="
rm -rf "$OUT_DIR"
mkdir -p "$OUT_DIR"

# Copy exe
EXE_PATH=$(find "$BUILD_DIR" -name "FPSGame.exe" | head -1)
if [ -z "$EXE_PATH" ]; then
    echo "ERROR: FPSGame.exe not found after build!"
    exit 1
fi
cp "$EXE_PATH" "$OUT_DIR/FPSGame.exe"
echo "  Copied FPSGame.exe"

# Copy SDL2.dll (built from source)
SDL2_DLL=$(find "$BUILD_DIR" -name "SDL2.dll" | head -1)
if [ -n "$SDL2_DLL" ]; then
    cp "$SDL2_DLL" "$OUT_DIR/SDL2.dll"
    echo "  Copied SDL2.dll"
else
    echo "WARNING: SDL2.dll not found in build dir, searching system..."
    # Fallback: look for MinGW SDL2.dll on system
    SYS_SDL2=$(find /usr/x86_64-w64-mingw32 -name "SDL2.dll" 2>/dev/null | head -1)
    if [ -n "$SYS_SDL2" ]; then
        cp "$SYS_SDL2" "$OUT_DIR/SDL2.dll"
        echo "  Copied SDL2.dll from system MinGW"
    else
        echo "WARNING: SDL2.dll not found! Windows build may not run without it."
    fi
fi

# Copy MinGW runtime DLLs (needed if not statically linked)
for dll in libwinpthread-1.dll libgcc_s_seh-1.dll libstdc++-6.dll; do
    DLL_PATH=$(find /usr/x86_64-w64-mingw32 -name "$dll" 2>/dev/null | head -1)
    if [ -n "$DLL_PATH" ]; then
        cp "$DLL_PATH" "$OUT_DIR/$dll"
        echo "  Copied $dll"
    fi
done

# Copy resources folder if it exists
if [ -d "$SCRIPT_DIR/resources" ] && [ "$(ls -A "$SCRIPT_DIR/resources" 2>/dev/null)" ]; then
    cp -r "$SCRIPT_DIR/resources" "$OUT_DIR/resources"
    echo "  Copied resources/"
fi

# Create a launcher readme
cat > "$OUT_DIR/README.txt" << 'README'
FPS Game - Windows Build
========================

Controls:
  WASD       - Move
  Mouse      - Look around
  Left Click - Shoot
  R          - Reload
  Space      - Jump
  ESC        - Quit

Requirements:
  - Windows 10 or later
  - GPU with OpenGL 3.3+ support (update drivers if game won't start)

To play:
  Double-click FPSGame.exe

All DLLs are included — no installation required!

Troubleshooting:
  "Missing DLL" error   → Make sure SDL2.dll is in the same folder as FPSGame.exe
  Black screen / crash  → Update your GPU drivers (need OpenGL 3.3+)
  OpenGL error          → Your GPU may not support OpenGL 3.3 Core Profile
README
echo "  Created README.txt"

echo ""
echo "=== Contents of windows-build/ ==="
ls -lh "$OUT_DIR/"

# --------------------------------------------------------------------------
# 7. Zip it up
# --------------------------------------------------------------------------
echo ""
echo "=== Creating zip: $ZIP_NAME ==="
rm -f "$SCRIPT_DIR/$ZIP_NAME"
cd "$SCRIPT_DIR"
zip -r "$ZIP_NAME" "windows-build/" 2>&1
echo ""
echo "======================================================"
echo " SUCCESS!"
echo "======================================================"
echo " Executable : windows-build/FPSGame.exe"
echo " ZIP file   : $ZIP_NAME"
echo ""
echo " Copy the ZIP to Windows, extract, and run FPSGame.exe"
echo "======================================================"

# Cleanup temp build dir
rm -rf "$BUILD_DIR"
