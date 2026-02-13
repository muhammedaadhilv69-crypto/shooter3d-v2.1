#!/bin/bash

echo "=== Downloading SDL2 Source for Portable Build ==="

SDL2_VERSION="2.28.5"
SDL2_URL="https://github.com/libsdl-org/SDL/releases/download/release-${SDL2_VERSION}/SDL2-${SDL2_VERSION}.tar.gz"

cd external

# Download SDL2 if not present
if [ ! -d "SDL2" ]; then
    echo "Downloading SDL2 ${SDL2_VERSION}..."
    
    if command -v wget &> /dev/null; then
        wget -q --show-progress "${SDL2_URL}" -O SDL2.tar.gz
    elif command -v curl &> /dev/null; then
        curl -L "${SDL2_URL}" -o SDL2.tar.gz
    else
        echo "Error: Neither wget nor curl found. Please install one."
        exit 1
    fi
    
    echo "Extracting SDL2..."
    tar -xzf SDL2.tar.gz
    mv SDL2-${SDL2_VERSION} SDL2
    rm SDL2.tar.gz
    
    echo "SDL2 downloaded and extracted successfully!"
else
    echo "SDL2 already exists in external/SDL2"
fi

echo ""
echo "=== SDL2 Setup Complete ==="
echo "You can now build with: ./build_portable.sh"
