#!/bin/bash

echo "=== Building Portable FPS Game ==="
echo ""

# Check if SDL2 source exists
if [ ! -d "external/SDL2" ]; then
    echo "SDL2 source not found. Downloading..."
    ./download_sdl2.sh
    if [ $? -ne 0 ]; then
        echo "Failed to download SDL2. Please run ./download_sdl2.sh manually."
        exit 1
    fi
fi

# Create build directory
mkdir -p build-portable
cd build-portable

echo ""
echo "Configuring build..."
cmake .. -DCMAKE_BUILD_TYPE=Release

if [ $? -ne 0 ]; then
    echo ""
    echo "CMake configuration failed!"
    exit 1
fi

echo ""
echo "Building (this will take a few minutes as SDL2 is compiled from source)..."
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo ""
    echo "Build failed!"
    exit 1
fi

echo ""
echo "=== Build Complete! ==="
echo ""
echo "Portable game location: build-portable/bin/"
echo "Files created:"
ls -lh bin/
echo ""
echo "To run: cd build-portable/bin && ./FPSGame"
echo ""
echo "You can copy the entire 'bin' folder to any machine!"
echo "All required libraries (SDL2.dll/so) are included."
