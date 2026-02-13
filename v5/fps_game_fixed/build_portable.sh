#!/usr/bin/env bash
# Build FPS Game natively (Linux/macOS)
set -e

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$SCRIPT_DIR"

# Check for SDL2
if [ ! -d "external/SDL2" ]; then
    echo "SDL2 not found. Running download script..."
    bash ./download_sdl2.sh
fi

echo "=== Configuring CMake (native build) ==="
mkdir -p build-native
cmake -S . -B build-native \
    -DCMAKE_BUILD_TYPE=Release \
    -DSDL_SHARED=ON \
    -DSDL_STATIC=OFF \
    -DSDL_TEST=OFF

echo "=== Building ==="
CORES=$(nproc 2>/dev/null || sysctl -n hw.ncpu 2>/dev/null || echo 2)
cmake --build build-native -j"$CORES"

echo ""
echo "=== Done! ==="
echo "Run with: ./build-native/bin/FPSGame"
