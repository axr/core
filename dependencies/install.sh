#!/bin/sh

# Installs dependencies for building AXR

if [ "`uname`" == "Darwin" ] ; then
    sudo port install qt4-mac +debug +framework +quartz +universal
    sudo port install boost +universal
    sudo port install libsdl-devel +universal
    sudo port install expat +universal
    sudo port install cairo-devel +quartz +universal
    sudo port install pango-devel +quartz +universal
elif [ "`uname`" == "Linux" ] ; then
    # For Arch Linux
    which pacman > /dev/null 2>&1
    if [ $? -eq 0 ] ; then
        sudo pacman -S qt boost sdl expat cairo pango
    else
        echo "TODO: Install necessary Debian/RPM packages"
    fi
else
    echo "Unknown platform - you will have to find which packages to install manually, or build from source"
    echo "- Qt 4 (once the new prototype app is developed)"
    echo "- Boost"
    echo "- SDL"
    echo "- Expat"
    echo "- Cairo"
    echo "- Pango"
fi
