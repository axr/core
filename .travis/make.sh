#!/bin/bash
set -e

# Build all variants
cd ..
make -C build-shared all docs package_source #package
make -C build-static all docs package_source #package
