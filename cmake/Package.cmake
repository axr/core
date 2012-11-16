set(AXR_PACKAGE_NAME "AXR")
set(AXR_PACKAGE_PREFIX "axr")

if(WIN32)
    set(AXR_PACKAGE_ICON "${CMAKE_SOURCE_DIR}\\\\share\\\\icons\\\\prototype.ico")
elseif(APPLE)
    set(AXR_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/share/icons/prototype.icns")
else()
    set(AXR_PACKAGE_ICON "${CMAKE_SOURCE_DIR}/share/icons/prototype.png")
endif()

# Per-generator overrides
set(CPACK_PROJECT_CONFIG_FILE "${CMAKE_BINARY_DIR}/PackageOverrides.cmake")

# PackageMaker only allows .rtf, .rtfd, .html and .txt
configure_file("${CMAKE_SOURCE_DIR}/cmake/PackageOverrides.cmake.in" "${CPACK_PROJECT_CONFIG_FILE}")
configure_file("${AXR_README_FILE}" "${AXR_README_FILE_TXT}")
configure_file("${AXR_LICENSE_FILE}" "${AXR_LICENSE_FILE_TXT}")

# Package version
set(CPACK_PACKAGE_VERSION_MAJOR ${AXR_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${AXR_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${AXR_VERSION_PATCH})
set(CPACK_PACKAGE_VERSION_BUILD ${AXR_VERSION_BUILD})
set(CPACK_PACKAGE_VERSION ${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}.${CPACK_PACKAGE_VERSION_BUILD})

# CPack variables...
set(CPACK_PACKAGE_ICON "${AXR_PACKAGE_ICON}")
set(CPACK_PACKAGE_NAME "${AXR_PACKAGE_NAME}")
set(CPACK_PACKAGE_DESCRIPTION "<TODO: insert first paragraph from README>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "The rendering engine that powers the AXR platform.")
set(CPACK_PACKAGE_VENDOR "${AXR_VENDOR}")
set(CPACK_PACKAGE_CONTACT "${AXR_CONTACT}")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${AXR_README_FILE_TXT}")
set(CPACK_RESOURCE_FILE_README "${AXR_README_FILE_TXT}")
set(CPACK_RESOURCE_FILE_LICENSE "${AXR_LICENSE_FILE_TXT}")

# Installation types...
set(CPACK_ALL_INSTALL_TYPES Full Developer)

# Friendly names, descriptions and groups for components
set(CPACK_COMPONENT_BROWSER_DISPLAY_NAME "Browser Application")
set(CPACK_COMPONENT_BROWSER_DESCRIPTION "An browser-like application that can be used to test the AXR rendering engine and HSS language")
set(CPACK_COMPONENT_BROWSER_GROUP "Runtime")
set(CPACK_COMPONENT_BROWSER_INSTALL_TYPES Full Developer)
set(CPACK_COMPONENT_BROWSER_REQUIRED TRUE)

set(CPACK_COMPONENT_LIBRARIES_GROUP "Runtime")
set(CPACK_COMPONENT_LIBRARIES_REQUIRED TRUE)
set(CPACK_COMPONENT_LIBRARIES_INSTALL_TYPES Full Developer)
set(CPACK_COMPONENT_LIBRARIES_HIDDEN TRUE)

set(CPACK_COMPONENT_FRAMEWORKS_GROUP "Runtime")
set(CPACK_COMPONENT_FRAMEWORKS_REQUIRED TRUE)
set(CPACK_COMPONENT_FRAMEWORKS_INSTALL_TYPES Full Developer)
set(CPACK_COMPONENT_FRAMEWORKS_HIDDEN TRUE)

set(CPACK_COMPONENT_EXAMPLES_DISPLAY_NAME "Example Applications")
set(CPACK_COMPONENT_EXAMPLES_DESCRIPTION "Example programs demonstrating how the AXR core library can be used")
set(CPACK_COMPONENT_EXAMPLES_GROUP "Development")
set(CPACK_COMPONENT_EXAMPLES_INSTALL_TYPES Developer)

set(CPACK_COMPONENT_DOC_DISPLAY_NAME "Documentation")
set(CPACK_COMPONENT_DOC_DESCRIPTION "Documentation for the AXR SDK and examples")
set(CPACK_COMPONENT_DOC_GROUP "Development")
set(CPACK_COMPONENT_DOC_INSTALL_TYPES Developer)

# Describe component groups
set(CPACK_COMPONENT_GROUP_RUNTIME_DESCRIPTION "")
set(CPACK_COMPONENT_GROUP_DEVELOPMENT_DESCRIPTION "All of the tools required to develop applications using AXR.\n\nNote that the Qt SDK and any other external dependencies must be installed separately, as well as a compiler environment (such as Visual Studio or Xcode).")

# Filenames
include(PackageFilenames)
set(CPACK_SOURCE_IGNORE_FILES ".git" ".DS_Store" "thumbs.db" "CMakeLists.txt.user")

# Source package
list(APPEND CPACK_SOURCE_GENERATOR ZIP TGZ STGZ TZ TBZ2)

# for NSIS to create Start Menu shortcuts
set(CPACK_PACKAGE_EXECUTABLES "Browser;Browser")

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_MUI_ICON "${AXR_PACKAGE_ICON}")
    set(CPACK_NSIS_MUI_UNIICON "${AXR_PACKAGE_ICON}")
#    set(CPACK_PACKAGE_ICON "<something>.bmp")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "browser.exe")
    set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\axr.vg")
    set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\axr.vg")
    set(CPACK_NSIS_CONTACT "${AXR_CONTACT}")

    # NSIS assumes executables are in bin by default
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY .)
endif()

if(APPLE)
    set(CPACK_GENERATOR "PackageMaker")
    set(CPACK_DMG_VOLUME_NAME "${AXR_PACKAGE_NAME} ${AXR_VERSION_STRING}")
    set(CPACK_OSX_PACKAGE_VERSION ${CMAKE_OSX_DEPLOYMENT_TARGET})
endif()

if(DPKG_FOUND)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "qt-sdk (>= 4.8)")
    set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${AXR_VENDOR} <${AXR_CONTACT}>")
    set(CPACK_DEBIAN_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
    set(CPACK_DEBIAN_PACKAGE_PRIORITY "optional")
    set(CPACK_DEBIAN_PACKAGE_SECTION "Development")
    set(CPACK_DEBIAN_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}") # add +lenny1 for example, for distro-specific pkgs
endif()

if(RPMBUILD_FOUND)
    set(CPACK_GENERATOR "RPM")
    set(CPACK_RPM_PACKAGE_DEPENDS "qt ( >= 4.6), qt-x11 ( >= 4.6 )")
    set(CPACK_RPM_PACKAGE_DESCRIPTION "${CPACK_PACKAGE_DESCRIPTION}")
    set(CPACK_RPM_PACKAGE_GROUP "Development/Libraries")
    set(CPACK_RPM_PACKAGE_LICENSE "GPLv3 License")
    set(CPACK_RPM_PACKAGE_NAME "${CPACK_PACKAGE_NAME}")
    set(CPACK_RPM_PACKAGE_PROVIDES "axr-sdk")
    set(CPACK_RPM_PACKAGE_REQUIRES "qt >= 4.8")
    set(CPACK_RPM_PACKAGE_SUMMARY "${CPACK_PACKAGE_DESCRIPTION_SUMMARY}")
    set(CPACK_RPM_PACKAGE_VENDOR "${CPACK_PACKAGE_VENDOR}")
    set(CPACK_RPM_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}") # long version string specifically?
endif()
