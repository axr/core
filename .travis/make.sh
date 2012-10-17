#!/bin/bash
set -e

# Build all variants
cd ..

# TODO: Source packaging is currently broken due to
# Firebreath files (filenames too long?)
make -C build-shared all package #package_source
make -C build-static all package #package_source
