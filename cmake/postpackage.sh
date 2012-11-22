#!/bin/bash
set -e

cpack_rpm_dir=@CMAKE_BINARY_DIR@/_CPack_Packages/@CMAKE_SYSTEM_NAME@/RPM

if [ -d "$cpack_rpm_dir" ] ; then
    components=('doc' 'headers' 'libraries')

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
