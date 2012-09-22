# for NSIS to create Start Menu shortcuts
#set(CPACK_PACKAGE_EXECUTABLES "${TARGET_NAME};${CMAKE_PROJECT_NAME}")

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
set(CPACK_PACKAGE_DESCRIPTION "")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "")
set(CPACK_PACKAGE_VENDOR "${AXR_VENDOR}")
set(CPACK_PACKAGE_CONTACT "${AXR_CONTACT}")
set(CPACK_PACKAGE_DESCRIPTION_FILE "${AXR_README_FILE_TXT}")
set(CPACK_RESOURCE_FILE_README "${AXR_README_FILE_TXT}")
set(CPACK_RESOURCE_FILE_LICENSE "${AXR_LICENSE_FILE_TXT}")

# Installation types...
set(CPACK_ALL_INSTALL_TYPES Full Developer)

# Friendly names, descriptions and groups for components
set(CPACK_COMPONENT_PROTOTYPE_DISPLAY_NAME "Prototype Application")
set(CPACK_COMPONENT_PROTOTYPE_DESCRIPTION "An pseudo-browser that can be used to test the AXR rendering engine and HSS language")
set(CPACK_COMPONENT_PROTOTYPE_GROUP "Runtime")
set(CPACK_COMPONENT_PROTOTYPE_INSTALL_TYPES Full Developer)
set(CPACK_COMPONENT_PROTOTYPE_REQUIRED TRUE)

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
set(CPACK_COMPONENT_GROUP_DEVELOPMENT_DESCRIPTION "All of the tools required to develop applications using AXR.\n\nNote that the Qt SDK and Boost C++ libraries must be installed separately, as well as a compiler environment (such as Visual Studio or Xcode).")

# Platform name
string(TOLOWER "${CMAKE_SYSTEM_NAME}" OS_CODE)
if(APPLE)
    set(OS_CODE "osx")
endif()

# Filenames
set(CPACK_PACKAGE_FILE_NAME "${AXR_PACKAGE_PREFIX}-${AXR_VERSION_STRING}-${OS_CODE}-${CMAKE_TARGET_ARCHITECTURE_CODE}")
set(CPACK_SOURCE_PACKAGE_FILE_NAME "${AXR_PACKAGE_PREFIX}-${AXR_VERSION_STRING}-src")
set(CPACK_SOURCE_IGNORE_FILES ".git" ".DS_Store" "thumbs.db" "CMakeLists.txt.user")

# Source package
list(APPEND CPACK_SOURCE_GENERATOR "ZIP")
if(UNIX)
    list(APPEND CPACK_SOURCE_GENERATOR TGZ STGZ TBZ2 TZ)
endif()

if(WIN32)
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_NSIS_MUI_ICON "${AXR_PACKAGE_ICON}")
    set(CPACK_NSIS_MUI_UNIICON "${AXR_PACKAGE_ICON}")
#    set(CPACK_PACKAGE_ICON "<something>.bmp")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\prototype.exe")
    set(CPACK_NSIS_HELP_LINK "http:\\\\\\\\axr.vg")
    set(CPACK_NSIS_URL_INFO_ABOUT "http:\\\\\\\\axr.vg")
    set(CPACK_NSIS_CONTACT "${AXR_CONTACT}")
endif()

if(APPLE)
    set(CPACK_GENERATOR "DragNDrop;PackageMaker")
    set(CPACK_DMG_VOLUME_NAME "${AXR_PACKAGE_NAME} ${AXR_VERSION_STRING}")
    set(CPACK_OSX_PACKAGE_VERSION ${CMAKE_OSX_DEPLOYMENT_TARGET})
endif()

if(__DEB)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${AXR_VENDOR} <${AXR_CONTACT}>")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "qt-sdk (>= 4.8), libboost") # TODO: probably the wrong Boost package
    set(CPACK_DEBIAN_PACKAGE_SECTION "Development")
    set(CPACK_DEBIAN_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION}+lenny1")
endif()

if(__RPM)
    set(CPACK_GENERATOR "RPM")
    set(CPACK_RPM_PACKAGE_REQUIRES "qt >= 4.8")
    set(CPACK_RPM_PACKAGE_PROVIDES "axr-sdk")
endif()
