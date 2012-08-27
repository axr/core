#!/bin/sh
set -e

if [ "$(id -u)" != "0" ] ; then
    echo "This script must be run as root" 1>&2
    exit 1
fi

# Installs dependencies for building AXR
UNAME=$(uname)

deps()
{
    echo "- Qt 4"
    echo "- Boost"
    echo "- SDL"
    echo "- Expat"
    echo "- Cairo"
    echo "- Pango"
}

doc()
{
    echo "Please install the following dependencies manually, either through"
    echo "your platform's native package manager or by building from source:"
    deps
}

if [ "$UNAME" = "Darwin" ] ; then
    # Target the earliest Intel version of OS X for maximum compatibility
    export MACOSX_DEPLOYMENT_TARGET=10.4

    # TODO: Allow the user to prefer a particular package manager if they have multiple
    if [ $(which port 2>/dev/null) ] ; then # MacPorts
        # NOTE: Qt +debug is BOTH debug and release
        port install \
            qt4-mac +debug +framework +quartz +universal \
            boost +no-static +universal \
            libsdl +universal \
            expat +universal \
            cairo +quartz +universal \
            pango +quartz +universal
    elif [ $(which brew 2>/dev/null) ] ; then # Brew
        # brew install ...
        echo "ERROR: brew support is not yet implemented"
        exit 1
    elif [ $(which fink 2>/dev/null) ] ; then # Fink
        # fink install ...
        echo "ERROR: fink support is not yet implemented"
        exit 1
    else
        echo "ERROR: Could not find any OS X package managers installed"
        echo "Please install one (we support MacPorts, Homebrew and Fink) and install"
        echo "the following dependencies, or build from source instead:"
        deps
        exit 1
    fi
elif [ "$UNAME" = "Linux" ] ; then
    if [ $(which apt-get 2>/dev/null) ] ; then
        # Debian, Ubuntu
        apt-get install qt-sdk libboost-all-dev libsdl1.2-dev libexpat1-dev libcairo2-dev libpango1.0-dev
    elif [ $(which yum 2>/dev/null) ] ; then
        # Fedora, RHEL, Yellow Dog Linux
        echo "ERROR: yum support is not yet implemented"
        exit 1
    elif [ $(which zypper 2>/dev/null) ] ; then
        # openSUSE
        echo "ERROR: zypper support is not yet implemented"
        exit 1
    elif [ $(which slackpkg 2>/dev/null) ] ; then
        # Slackware
        echo "ERROR: slackpkg support is not yet implemented"
        exit 1
    elif [ $(which pacman 2>/dev/null) ] ; then
        # Arch Linux
        pacman -S qt boost sdl expat cairo pango
    elif [ $(which emerge 2>/dev/null) ] ; then
        # Gentoo
        echo "ERROR: portage support is not yet implemented"
        exit 1
    else
        echo "ERROR: Unknown package manager!"
        doc
        exit 1
    fi
elif [ "$UNAME" = "FreeBSD" ] ; then
    # pkg_add -r ...
    echo "ERROR: FreeBSD support is not yet implemented"
    exit 1
else
    echo "ERROR: Unknown platform!"
    doc
    exit 1
fi
