#!/bin/bash

echo "Building resources directory..."

#if [ -d ./rsrc/ ]; then
#	rm -rf ./rsrc/
#fi

ln -sf ../resources/ ./rsrc

echo "Done"

