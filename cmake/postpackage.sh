#!/bin/bash
set -e

cpack_rpm_dir=@CMAKE_BINARY_DIR@/_CPack_Packages/@CMAKE_SYSTEM_NAME@/RPM

if [ -d "$cpack_rpm_dir" ] ; then
    components=('doc' 'headers' 'libraries')

    for component in $components ; do
        builddir=$cpack_rpm_dir/@CPACK_PACKAGE_FILE_NAME@/$component
        spec=$cpack_rpm_dir/SPECS/@AXR_PACKAGE_PREFIX@-$component.spec
        cd $builddir
        rpmbuild -bb --define "_topdir $cpack_rpm_dir" --buildroot "$builddir" "$spec"
    done
fi
