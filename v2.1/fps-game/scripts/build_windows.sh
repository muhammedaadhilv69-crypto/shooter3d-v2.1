#!/bin/bash

# Build script for Windows (cross-compilation from Linux/Codespaces)

set -e

echo "=== Building FPS Shooter for Windows (Cross-Compilation) ==="

cd "$(dirname "$0")/.."

# Check for MinGW
if ! command -v x86_64-w64-mingw32-gcc &> /dev/null; then
    echo "Installing MinGW cross-compiler..."
    sudo apt-get update
    sudo apt-get install -y mingw-w64
fi

# Fetch dependencies
bash scripts/fetch_deps.sh

# Create build directory
mkdir -p build/windows
cd build/windows

# Configure with CMake for Windows cross-compilation
cmake ../.. \
    -DCMAKE_TOOLCHAIN_FILE=../../cmake/Toolchain-mingw64.cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_AUDIO=OFF

# Build
make -j$(nproc)

echo "=== Build Complete ==="
echo "Executable: build/windows/FPSShooter.exe"
