#!/bin/bash
set -e

# Build all variants
cd ..
make -C build-shared all package package_source
make -C build-static all package package_source
