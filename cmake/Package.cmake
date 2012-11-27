set(AXR_PACKAGE_NAME "AXR")

if(WIN32 OR APPLE)
    set(AXR_PACKAGE_PREFIX "axr-runtime")
elseif(DPKG_FOUND)
    set(AXR_PACKAGE_NAME "libaxr") # CPACK_DEBIAN_PACKAGE_NAME doesn't work
    set(AXR_PACKAGE_PREFIX "libaxr")
else()
    set(AXR_PACKAGE_PREFIX "axr")
endif()

if(WIN32)
    set(AXR_PACKAGE_ICON "${CMAKE_SOURCE_DIR}\\\\share\\\\icons\\\\prototype.ico")
elseif(APPLE)
    set(AXR_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/share/icons/prototype.icns")
else()
    set(AXR_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/share/icons/prototype.png")
endif()

# Per-generator overrides
set(CPACK_PROJECT_CONFIG_FILE "${CMAKE_BINARY_DIR}/PackageOverrides.cmake")
configure_file("${CMAKE_SOURCE_DIR}/cmake/PackageOverrides.cmake.in" "${CPACK_PROJECT_CONFIG_FILE}")

# PackageMaker only allows .rtf, .rtfd, .html and .txt
configure_file("${AXR_README_FILE}" "${AXR_README_FILE_TXT}")
configure_file("${AXR_LICENSE_FILE}" "${AXR_LICENSE_FILE_TXT}")

# Package version
set(CPACK_PACKAGE_VERSION_MAJOR ${AXR_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${AXR_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${AXR_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION_BUILD ${AXR_VERSION_BUILD})
set(CPACK_PACKAGE_VERSION ${AXR_VERSION_STRING_REAL})

# CPack variables...
set(CPACK_PACKAGE_ICON "${AXR_PACKAGE_ICON}")
set(CPACK_PACKAGE_NAME "${AXR_PACKAGE_NAME}")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${CMAKE_SOURCE_DIR}/common/description.txt")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Rendering engine that powers the AXR platform")
set(CPACK_PACKAGE_VENDOR "${AXR_VENDOR}")
set(CPACK_PACKAGE_CONTACT "${AXR_CONTACT}")
set(CPACK_RESOURCE_FILE_README "${AXR_README_FILE_RTF}")
set(CPACK_RESOURCE_FILE_LICENSE "${AXR_LICENSE_FILE_TXT}")
set(CPACK_STRIP_FILES ON)

# Friendly names, descriptions and groups for components
set(CPACK_COMPONENT_LIBRARIES_DISPLAY_NAME "Libraries")
set(CPACK_COMPONENT_LIBRARIES_DESCRIPTION "Library (.dll, .so, etc.) files")
set(CPACK_COMPONENT_LIBRARIES_REQUIRED TRUE)

set(CPACK_COMPONENT_HEADERS_DISPLAY_NAME "C/C++ Header Files")
set(CPACK_COMPONENT_HEADERS_DESCRIPTION "Header files for developing AXR C/C++ programs")

set(CPACK_COMPONENT_DOC_DISPLAY_NAME "Documentation")
set(CPACK_COMPONENT_DOC_DESCRIPTION "Documentation for the AXR SDK and examples")

# Filenames
include(PackageFilenames)
package_file_name(CPACK_PACKAGE_FILE_NAME "${AXR_PACKAGE_PREFIX}" "${AXR_VERSION_STRING}")
src_package_file_name(CPACK_SOURCE_PACKAGE_FILE_NAME "${AXR_PACKAGE_PREFIX}" "${AXR_VERSION_STRING}")
set(CPACK_SOURCE_IGNORE_FILES ".git" ".DS_Store" "thumbs.db" "CMakeLists.txt.user")

# Source package
list(APPEND CPACK_SOURCE_GENERATOR ZIP TGZ STGZ TZ TBZ2)

if(WIN32)
    # WiX packaging introduced in version 2.8.11... if we don't have that we'll fall back to NSIS
    if(${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_EQUAL 2.8.11 OR
       ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION}.${CMAKE_PATCH_VERSION} VERSION_GREATER 2.8.11)
        set(CPACK_GENERATOR "WIX")
    else()
        set(CPACK_GENERATOR "NSIS")
    endif()

    set(CPACK_NSIS_CONTACT "${AXR_CONTACT}")
    set(CPACK_NSIS_DISPLAY_NAME "${AXR_PACKAGE_NAME}")
    set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY .) # NSIS assumes executables are in bin by default
    set(CPACK_NSIS_HELP_LINK "${AXR_WEB_URL}")
    set(CPACK_NSIS_MODIFY_PATH ON)
    set(CPACK_NSIS_MUI_ICON "${AXR_PACKAGE_ICON}")
    set(CPACK_NSIS_MUI_UNIICON "${AXR_PACKAGE_ICON}") # TODO: This should probably be changed to an icon with a crossed out symbol
    set(CPACK_NSIS_PACKAGE_NAME "${AXR_PACKAGE_NAME}")
    set(CPACK_NSIS_URL_INFO_ABOUT "${AXR_WEB_URL}")

    # This is black magic. CPack has no way to let us place BrandingText in the
    # main section of the .nsi file... but SetCompressor IS in the right spot,
    # so we'll inject our own property after this one by using a newline
    set(CPACK_NSIS_COMPRESSOR "lzma
BrandingText '${AXR_VENDOR}'")

    set(CPACK_WIX_PRODUCT_GUID "XXXXXXXX-XXXX-XXXX-XXXX-XXXXXXXXXXXX") # Change on each release
    set(CPACK_WIX_UPGRADE_GUID "AFBAE4BB-0F7B-495D-87BE-F7CB636C24DB") # Permanent, never change this

    set(CPACK_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/common\\\\nsis-header.bmp")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "${AXR_PACKAGE_NAME}")
endif()

if(APPLE)
    set(CPACK_GENERATOR "PackageMaker")
    set(CPACK_DMG_VOLUME_NAME "${AXR_PACKAGE_NAME} ${AXR_VERSION_STRING}")
    set(CPACK_OSX_PACKAGE_VERSION ${CMAKE_OSX_DEPLOYMENT_TARGET})
endif()

include(PackageLinux)

if(DPKG_FOUND)
    file(READ "${CPACK_PACKAGE_DESCRIPTION_FILE}" CPACK_PACKAGE_DESCRIPTION)

    # Finicky Debian control file formatting...
    string(REPLACE "\n\n" "\n.\n" CPACK_PACKAGE_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION}")
    string(REPLACE "\n" "\n " CPACK_PACKAGE_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION}")
    string(STRIP "${CPACK_PACKAGE_DESCRIPTION}" CPACK_PACKAGE_DESCRIPTION)

    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6, libqtcore4 (>= 4.8), libqtgui4 (>= 4.8), libqt4-network (>= 4.8), libqt4-svg (>= 4.8), libqt4-xml (>= 4.8)")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}\n ${CPACK_PACKAGE_DESCRIPTION}")
    set(CPACK_DEBIAN_PACKAGE_HOMEPAGE "${AXR_WEB_URL}")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${AXR_VENDOR} <${AXR_CONTACT}>")
    set(CPACK_DEBIAN_PACKAGE_NAME "${AXR_PACKAGE_PREFIX}")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    set(CPACK_DEBIAN_PACKAGE_SECTION "devel")
    set(CPACK_DEBIAN_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}")

    file(WRITE "${CMAKE_BINARY_DIR}/postinst" "ldconfig")
    execute_process(COMMAND chmod 755 "${CMAKE_BINARY_DIR}/postinst")
    set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_BINARY_DIR}/postinst")

    install(FILES "${CMAKE_BINARY_DIR}/copyright" DESTINATION share/doc/libaxr RENAME copyright COMPONENT libraries)
    install(FILES "${CMAKE_BINARY_DIR}/deb/changelog.gz" DESTINATION share/doc/libaxr COMPONENT libraries)

    install(FILES "${CMAKE_BINARY_DIR}/copyright" DESTINATION share/doc/libaxr-dev RENAME copyright COMPONENT headers)
    install(FILES "${CMAKE_BINARY_DIR}/deb/changelog.gz" DESTINATION share/doc/libaxr-dev COMPONENT headers)

    install(FILES "${CMAKE_BINARY_DIR}/copyright" DESTINATION share/doc/libaxr-doc RENAME copyright COMPONENT doc)
    install(FILES "${CMAKE_BINARY_DIR}/deb/changelog.gz" DESTINATION share/doc/libaxr-doc COMPONENT doc)
endif()

if(RPMBUILD_FOUND)
    set(CPACK_GENERATOR "RPM")
    set(CPACK_RPM_CHANGELOG_FILE "${CMAKE_BINARY_DIR}/rpm/changelog")
    set(CPACK_RPM_PACKAGE_ARCHITECTURE "${ARCH_CODE}")
    set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
    set(CPACK_RPM_PACKAGE_LICENSE "GPL")
    set(CPACK_RPM_PACKAGE_NAME "${AXR_PACKAGE_PREFIX}")
    set(CPACK_RPM_PACKAGE_RELEASE 1) # Don't change this
    set(CPACK_RPM_PACKAGE_REQUIRES "qt >= 4.8, qt-x11 >= 4.8")
    set(CPACK_RPM_PACKAGE_URL "${AXR_WEB_URL}")
    set(CPACK_RPM_PACKAGE_VENDOR "${CPACK_PACKAGE_VENDOR}")
    set(CPACK_RPM_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}")
endif()

if(DPKG_FOUND OR RPMBUILD_FOUND)
    set(CMAKE_INSTALL_PREFIX /usr)
endif()
