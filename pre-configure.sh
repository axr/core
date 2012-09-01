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
    # Tiger's far too old to bother with; Leopard was really the beginning of modern OS X
    # and is the first virtualizable OS X operating system
    mp_target=10.5
    export MACOSX_DEPLOYMENT_TARGET=$mp_target

    # TODO: Allow the user to prefer a particular package manager if they have multiple
    if [ $(which port 2>/dev/null) ] ; then # MacPorts
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

        # NOTE: Qt +debug is BOTH debug and release
        port install \
            boost +no-static +universal \
            libsdl +universal \
            expat +universal \
            cairo +quartz +universal \
            pango +quartz +universal \
            qt4-mac +debug +framework +quartz +universal
    elif [ $(which brew 2>/dev/null) ] ; then # Brew
        # brew install cairo pango boost ...
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
        apt-get install build-essential qt-sdk libboost-all-dev libsdl1.2-dev libexpat1-dev libcairo2-dev libpango1.0-dev
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
