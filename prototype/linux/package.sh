#!/bin/bash

version="$1"

if [ -z $version ]; then
	version=$(git rev-parse --verify HEAD)-$(git branch | grep "*" | sed "s/* //")
fi

get_tutorial ()
{
	temp="$(mktemp -d)"
	git clone git://github.com/AXR/Extras.git "$temp"

	tutorial="$temp/Tutorials/000_tutorial_html/"
}

package ()
{
	mkdir ./package/

	cp ./axr_$1 ./package/
	cp -r ../tests/ ./package/
	cp -r "$tutorial" ./package/tutorial

	cd ./package/
	tar -czf ../axr_${version}_linux_$2.tar.gz ./*

	cd ..
	rm -rf ./package
}

echo "Packaging AXR protptype version $version"

echo "Cloning the tutorial from Extras repo..."
get_tutorial

# Build resources directory
./build_rsrc.sh

if [ -f ./axr_64 ]; then
	echo "x86-64 build found. Packaging..."
	package "64" "x86-64"
fi

if [ -f ./axr_32 ]; then
	echo "x86 build found. Packaging..."
	package "32" "x86"
fi

