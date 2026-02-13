#!/bin/bash

# Script to fetch dependencies for FPS Shooter

set -e

echo "=== Fetching Dependencies ==="

cd "$(dirname "$0")/.."
mkdir -p third_party
cd third_party

# Fetch GLFW if not present
if [ ! -d "glfw" ]; then
    echo "Fetching GLFW..."
    git clone --depth 1 --branch 3.3.8 https://github.com/glfw/glfw.git glfw
    echo "GLFW fetched successfully"
else
    echo "GLFW already present"
fi

# Fetch GLM if not present
if [ ! -d "glm" ]; then
    echo "Fetching GLM..."
    git clone --depth 1 --branch 0.9.9.8 https://github.com/g-truc/glm.git glm
    echo "GLM fetched successfully"
else
    echo "GLM already present"
fi

echo "=== Dependencies Ready ==="
