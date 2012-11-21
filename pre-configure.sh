#!/bin/bash
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
    echo "- CMake"
    echo "- GTK+ 2 (optional on Windows and OS X - required on X11)"
    echo "- SDL (optional)"
    echo "- Doxygen (optional)"
}

doc()
{
    echo "Please install the following dependencies manually, either through"
    echo "your platform's native package manager or by building from source:"
    deps
}

set_mp_target()
{
    mp_target=$1
    export MACOSX_DEPLOYMENT_TARGET=$mp_target

    # Determine MacPorts installation path
    mp="$(which port)"
    mp_prefix="${mp%/bin/port}"
    if [ -z "$mp_prefix" ] ; then
        echo "Could not determine MacPorts installation prefix"
        exit 1
    fi

    # Get MacPorts configuration file path
    mp_conf="$mp_prefix/etc/macports/macports.conf"

    # Check the MacPorts deployment target configuration and change it if necessary
    mp_deployment_target=$(egrep "^macosx_deployment_target\s+([0-9.]+)\s*$" "$mp_conf" | cut -d ' ' -f 2- | tr -d ' \t')
    if [ ! -z "$mp_deployment_target" ] ; then
        echo "Building MacPorts dependencies for OS X $mp_deployment_target+"
        if [ "$mp_deployment_target" != "$mp_target" ] ; then
            echo "MacPorts macosx_deployment_target setting was found ($mp_deployment_target) - changing to $mp_target..."
            sed -i '.bak' "s/macosx_deployment_target $mp_deployment_target/macosx_deployment_target $mp_target/g" "$mp_conf"
        fi
    else
        echo "MacPorts macosx_deployment_target setting was not found - setting to $mp_target..."
        echo "macosx_deployment_target $mp_target" | tee -a "$mp_conf" > /dev/null
    fi
}

if [ "$UNAME" = "Darwin" ] ; then
    osx_current=$(sw_vers -productVersion | cut -f 1-2 -d '.')
    if [ -z "$osx_current" ] ; then
        echo "Unable to determine current OS X version"
        exit 1
    fi

    # TODO: Allow the user to prefer a particular package manager if they have multiple
    if [ $(which port 2>/dev/null) ] ; then # MacPorts
        # NOTE: At least for Qt, +debug builds debug AND release libraries

        # depof:<port> does not allow to specify variants of the <port> itself,
        # and since variants may install extra dependencies depof:<port> may not
        # necessarily install all dependencies - however, comparing the output of
        # `port deps <port>` and `port deps <port> +variant1 +variant2 +variant3`
        # will allow you to tell whether additional dependencies that won't be picked
        # up by depof:<port> will need to be installed

        # Currently our selected variants of Qt don't require extra deps
        # so depof:qt4-mac will be sufficient

        set_mp_target 10.4
        port install \
            libsdl +universal \
            depof:qt4-mac +universal \
            depof:cmake +universal

        set_mp_target $osx_current
        port install doxygen gtk2 +universal

        # earliest OS X that Qt 4.8.x Cocoa will build on
        # NOTE: for some reason MacPorts' Qt won't work with
        # the framework variant
        set_mp_target 10.5
        port install qt4-mac +debug +quartz +universal

        # reset deployment target to 10.4 when finished
        set_mp_target 10.4
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
        apt-get update
        apt-get install build-essential qt-sdk cmake libsdl1.2-dev libgtk2.0-dev doxygen
    elif [ $(which yum 2>/dev/null) ] ; then
        # Fedora, RHEL, Yellow Dog Linux
        yum groupinstall 'Development Tools'
        yum install rpm qt-devel cmake SDL-devel gtk2-devel doxygen
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
        pacman -S qt cmake sdl gtk2 doxygen
    elif [ $(which emerge 2>/dev/null) ] ; then
        # Gentoo with portage
        emerge -n qt-gui cmake sdl doxygen gtk+
    elif [ $(which cave 2>/dev/null) ] ; then
        # Exherbo or Gentoo with paludis
        echo "ERROR: Paludis support is not yet implemented"
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
