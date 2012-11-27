#!/bin/bash
set -e

components=('doc' 'headers' 'libraries')

for component in "${components[@]}" ; do
    if [[ $component = 'libraries' ]] ; then
        pkgname=libaxr
    elif [[ $component = 'headers' ]] ; then
        pkgname=libaxr-dev
    else
        pkgname=libaxr-$component
    fi

    olddeb=@CPACK_PACKAGE_FILE_NAME@-$component.deb
    newdeb=${pkgname}_@AXR_VERSION_STRING@_@ARCH_CODE@.deb

    cd "@CMAKE_BINARY_DIR@/dist"

    if [ -f "$olddeb" ] ; then
        # Fix filename...
        mv "$olddeb" "$newdeb"

        # Deconstruct .deb
        rm -rf "$newdeb.tmp"
        dpkg-deb --raw-extract "$newdeb" "$newdeb.tmp"
        rm "$newdeb"
        
        cd "$newdeb.tmp"

        # Set permissions correctly since CPack doesn't do it...
        fakeroot chmod 0644 DEBIAN/control DEBIAN/md5sums
        fakeroot find "./@CMAKE_INSTALL_PREFIX@" -type d | xargs chmod 0755
        fakeroot chown -R root:root *

        # Fix package name in control file
        printf '%s\n' "g/Package:[[:blank:]]\{1,\}@AXR_PACKAGE_PREFIX@-$component.*/s//Package: $pkgname/" w | ed -s DEBIAN/control

        if [[ $component = 'doc' ]] ; then
            # Fix package architecture
            printf '%s\n' "g/Architecture:[[:blank:]]\{1,\}.*/s//Architecture: all/" w | ed -s DEBIAN/control

            # Fix package section
            printf '%s\n' "g/Section:[[:blank:]]\{1,\}.*/s//Section: doc/" w | ed -s DEBIAN/control
        fi

        if [[ $component = 'headers' ]] ; then
            # Fix package section
            printf '%s\n' "g/Section:[[:blank:]]\{1,\}.*/s//Section: libdevel/" w | ed -s DEBIAN/control
        fi

        cd ..

        # Repackage
        fakeroot dpkg --build "$newdeb.tmp" "$newdeb"
        rm -rf "$newdeb.tmp"

        # Check the .deb file for problems
        lintian "$newdeb"
    fi
done

cpack_rpm_dir=@CMAKE_BINARY_DIR@/_CPack_Packages/@CMAKE_SYSTEM_NAME@/RPM

if [ -d "$cpack_rpm_dir" ] ; then
    for component in "${components[@]}" ; do
        builddir=$cpack_rpm_dir/@CPACK_PACKAGE_FILE_NAME@/$component
        spec=$cpack_rpm_dir/SPECS/@AXR_PACKAGE_PREFIX@-$component.spec

        # Remove CPack's broken files
        oldrpm=$cpack_rpm_dir/@CPACK_PACKAGE_FILE_NAME@-$component.rpm
        rm "$oldrpm"
        rm @CMAKE_BINARY_DIR@/dist/@CPACK_PACKAGE_FILE_NAME@-$component.rpm

        # Fix the spec files to correct the package names
        if [[ $component = 'libraries' ]] ; then
            newname='@AXR_PACKAGE_PREFIX@'
        elif [[ $component = 'headers' ]] ; then
            newname='@AXR_PACKAGE_PREFIX@-devel'
        else
            newname="@AXR_PACKAGE_PREFIX@-$component"
        fi

        printf '%s\n' "g/Name:[[:blank:]]\{1,\}@AXR_PACKAGE_PREFIX@-$component.*/s//Name: $newname/" w | ed -s "$spec"

        cd $builddir
        rpmbuild -bb --define "_topdir $cpack_rpm_dir" --buildroot "$builddir" "$spec"

        real_rpmname=$(rpm -qp "$oldrpm")
        cp "$oldrpm" "@CMAKE_BINARY_DIR@/dist/$real_rpmname.rpm"
    done
fi
