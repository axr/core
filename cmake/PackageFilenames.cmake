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
    endif()

    # TODO: For Linux we might need to set this to i586 or i686,
    # but this depends on the distribution and version
elseif(CMAKE_TARGET_ARCHITECTURE_CODE STREQUAL "x86_64")
    if(WIN32)
        set(ARCH_CODE "x64")
    endif()
endif()

set(CPACK_PACKAGE_FILE_NAME "${AXR_PACKAGE_PREFIX}-${AXR_VERSION_STRING}-${OS_CODE}-${ARCH_CODE}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${AXR_PACKAGE_PREFIX}-${AXR_VERSION_STRING}-src")
