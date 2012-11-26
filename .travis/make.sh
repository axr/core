#!/bin/bash
set -e

# Build all variants
cd ..

build-targets()
{
    cmake --build $1 --config Release --target all
    cmake --build $1 --config Release --target distribution
    cmake --build $1 --config Release --target package_source
}

build-targets build-shared
build-targets build-static
