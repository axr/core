#!/bin/bash

version="$1"
base=$(pwd)

if [ -z $version ]; then
    version=$(git rev-parse --verify HEAD)-$(git branch | grep "*" | sed "s/* //")
fi

get_tutorial ()
{
    temp="$(mktemp -d)"
    git clone git://github.com/AXR/Extras.git "$temp"

    tutorial="$temp/Tutorials/000_tutorial_html/"
}

get_arch ()
{
    if [[ $(file $base/../axr) =~ 64-bit ]]; then
        echo "x86-64"
    else
        echo "x86"
    fi
}

package ()
{
    mkdir ./package/

    cp ../axr ./package/
    cp -r ../tests/ ./package/
    cp -r "$tutorial" ./package/tutorial
    cp -r ../resources/ ./package/resources

    cd ./package/
    tar -czf ../axr_${version}_linux_$(get_arch).tar.gz ./*

    cd ..
    rm -rf ./package
}

echo "Packaging AXR prototype version $version for $(get_arch)"

echo "Cloning the tutorial from Extras repo..."
get_tutorial

echo "Packaging..."
package
