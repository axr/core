#!/bin/bash
set -e

# Clone all submodules
git submodule update --init --recursive

# Make build directories for both shared and static mode
mkdir ../build-shared ../build-static

# Configure using CMake (shared)
cd ../build-shared
cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DAXR_ALL_WARNINGS=ON \
    -DBUILD_SHARED_LIBS=ON \
    ../core

# Configure using CMake (static)
cd ../build-static
cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DAXR_ALL_WARNINGS=ON \
    ../core
