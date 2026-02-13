#!/bin/bash

echo "=== Building FPS Game ==="

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
make -j$(nproc)

echo ""
echo "=== Build Complete ==="
echo "Executable: build/bin/FPSGame"
echo ""
echo "To run: cd build/bin && ./FPSGame"
