#!/bin/bash
set -e

# Clone all submodules
git submodule update --init --recursive

# Make build directories for both shared and static mode
mkdir ../build-shared ../build-static

args="-D CMAKE_BUILD_TYPE=Release -D AXR_ALL_WARNINGS=ON -D AXR_BUILD_CORE=ON -D AXR_BUILD_DOCS=ON"

# Configure using CMake (shared)
cd ../build-shared
cmake $args -D BUILD_SHARED_LIBS=ON ../core

# Configure using CMake (static)
cd ../build-static
cmake $args -D BUILD_SHARED_LIBS=OFF ../core
