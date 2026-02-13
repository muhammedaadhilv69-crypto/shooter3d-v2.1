#!/bin/bash

echo "=== Installing Dependencies ==="

# Check OS
if [ -f /etc/os-release ]; then
    . /etc/os-release
    OS=$ID
else
    OS=$(uname -s)
fi

echo "Detected OS: $OS"

case "$OS" in
    ubuntu|debian)
        echo "Installing for Debian/Ubuntu..."
        sudo apt-get update
        sudo apt-get install -y libsdl2-dev libgl1-mesa-dev cmake build-essential
        ;;
    fedora|rhel|centos)
        echo "Installing for Fedora/RHEL/CentOS..."
        sudo dnf install -y SDL2-devel mesa-libGL-devel cmake gcc-c++
        ;;
    arch|manjaro)
        echo "Installing for Arch/Manjaro..."
        sudo pacman -S --needed sdl2 mesa cmake base-devel
        ;;
    Darwin)
        echo "Installing for macOS..."
        brew install sdl2 cmake
        ;;
    *)
        echo "Unknown OS. Please install: SDL2, OpenGL, CMake manually"
        exit 1
        ;;
esac

echo "=== Dependencies Installed ==="
