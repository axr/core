#!/bin/bash
set -e

change-specfile-pkgname()
{
    component=$1
    newname=$2
    specfile=$3
    printf '%s\n' "g/Name:[[:blank:]][[:blank:]]*$component/c\\" "Name: $newname" w | ex -s $specfile
}

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

        change-specfile-pkgname "@AXR_PACKAGE_PREFIX@-$component" "$newname" "$specfile"

        cd $builddir
        rpmbuild -bb --define "_topdir $cpack_rpm_dir" --buildroot "$builddir" "$spec"

        real_rpmname=$(rpm -qp "$oldrpm")
        cp "$oldrpm" "@CMAKE_BINARY_DIR@/dist/$real_rpmname"
    done
fi
