# Platform name
string(TOLOWER "${CMAKE_SYSTEM_NAME}" OS_CODE)

if(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(OS_CODE "osx")
endif()

# Architecture name
set(ARCH_CODE "${CMAKE_TARGET_ARCHITECTURE_CODE}")

# The "canonical" identifiers we use in CMake (i386/x86_64) are the same as what OS X uses
if(CMAKE_TARGET_ARCHITECTURE_CODE STREQUAL "i386")
    if(WIN32)
        set(ARCH_CODE "x86")
    elseif(RPMBUILD_FOUND)
        # TODO: in some cases this may be i386 or i586 depending on the distribution...
        # There's also i486 and i786 which seem much less common
        set(ARCH_CODE "i686")
    endif()
elseif(CMAKE_TARGET_ARCHITECTURE_CODE STREQUAL "x86_64")
    if(WIN32)
        set(ARCH_CODE "x64")
    elseif(DPKG_FOUND)
        set(ARCH_CODE "amd64")
    endif()
endif()

function(package_file_name output_variable pfn_package_prefix pfn_version_string)
    if(DPKG_FOUND)
        set(${output_variable} "${pfn_package_prefix}_${pfn_version_string}_${ARCH_CODE}" PARENT_SCOPE)
    elseif(RPMBUILD_FOUND)
        # Note that the "1" is the RPM package release number
        # Due to the way we package and distribute this should probably never change
        # but it's necessary to note its presence here since it's an awkward way of doing things...
        set(${output_variable} "${pfn_package_prefix}-${pfn_version_string}-1.${ARCH_CODE}" PARENT_SCOPE)
    else()
        set(${output_variable} "${pfn_package_prefix}-${pfn_version_string}-${OS_CODE}-${ARCH_CODE}" PARENT_SCOPE)
    endif()
endfunction()

function(src_package_file_name output_variable spfn_package_prefix spfn_version_string)
    set(${output_variable} "${spfn_package_prefix}-${spfn_version_string}-src" PARENT_SCOPE)
endfunction()
