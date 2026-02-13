#!/bin/bash

# Build script for Linux (native compilation)

set -e

echo "=== Building FPS Shooter for Linux ==="

cd "$(dirname "$0")/.."

# Fetch dependencies
bash scripts/fetch_deps.sh

# Create build directory
mkdir -p build/linux
cd build/linux

# Configure with CMake
cmake ../.. \
    -DCMAKE_BUILD_TYPE=Release \
    -DENABLE_AUDIO=OFF

# Build
make -j$(nproc)

echo "=== Build Complete ==="
echo "Executable: build/linux/FPSShooter"
