if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "You should not build in-source. Use a directory outside the source tree or a subdirectory such as: ${CMAKE_SOURCE_DIR}/build")
endif()

# NOTE: Untested on Windows
function(check_pathnames)
    if(WIN32)
        set(separator ":")
    endif()

    set(path_regex "^([A-Za-z0-9._/-${separator}]+)$")

    if(NOT "${CMAKE_SOURCE_DIR}" MATCHES "${path_regex}" OR NOT "${CMAKE_BINARY_DIR}" MATCHES "${path_regex}")
        message(FATAL_ERROR "Ensure that the source and build paths contain only the following characters: alphanumeric, dash, underscore, slash, dot (and colon on Windows)")
    endif()
endfunction()

check_pathnames()

project(AXR)
cmake_minimum_required(VERSION 2.8)

set(AXR_VERSION_STRING "0.4.7")

# Default to building universal binaries on OS X only during release builds
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(BUILD_UNIVERSAL_BINARIES_DEFAULT ON)
else()
    set(BUILD_UNIVERSAL_BINARIES_DEFAULT OFF)
endif()

# If we're using an IDE build system such as Visual Studio or Xcode,
# use fancier target names such as "Documentation" vs "docs"
string(FIND "${CMAKE_GENERATOR}" "Visual Studio" found_vs)
string(FIND "${CMAKE_GENERATOR}" "CodeBlocks" found_blocks)
if (found_vs GREATER -1 OR found_blocks GREATER -1 OR "${CMAKE_GENERATOR}" STREQUAL "Xcode")
    set(FANCY_TARGET_NAMES ON)
endif()

option(AXR_NO_DEPRECATED_ENUMS "Disallow use of deprecated enum members" OFF)
option(BUILD_PROTOTYPE "Builds the prototype including libraries and demo app" ON)
option(BUILD_DOCS "Build documentation using doxygen" ON)
option(BUILD_EXAMPLES "Builds example programs showing usage of the AXR library" OFF)
option(BUILD_UNIVERSAL_BINARIES "Build OS X universal binaries (i386 + x86_64)" ${BUILD_UNIVERSAL_BINARIES_DEFAULT})
option(BUILD_SHARED_LIBS "Builds shared instead of static libraries" ON)

if(BUILD_PROTOTYPE)
    # Don't use static Boost because this will result in relocation errors due to
    # Boost not being compiled with -fPIC (at least on 64-bit Linux)
    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)

    find_package(Boost COMPONENTS thread system program_options REQUIRED)
    find_package(SDL REQUIRED)
    find_package(EXPAT REQUIRED expat>=2.0)

    # TODO: Avoid using pkg-config
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(PANGO REQUIRED pango)
    pkg_check_modules(CAIRO REQUIRED cairo)
    pkg_check_modules(PANGOCAIRO REQUIRED pangocairo)

    include_directories(
        ${Boost_INCLUDE_DIRS}
        ${SDL_INCLUDE_DIR}
        ${EXPAT_INCLUDE_DIRS}
        ${PANGO_INCLUDE_DIRS}
        ${CAIRO_INCLUDE_DIRS}
        ${PANGOCAIRO_INCLUDE_DIRS}
    )

    # TODO: Only necessary due to pkg-config usage
    link_directories(
        ${PANGO_LIBRARY_DIRS}
        ${CAIRO_LIBRARY_DIRS}
        ${PANGOCAIRO_LIBRARY_DIRS}
    )

    if(AXR_NO_DEPRECATED_ENUMS)
        add_definitions(-DAXR_NO_DEPRECATED_ENUMS)
    endif()

    if(BUILD_UNIVERSAL_BINARIES OR "${CMAKE_BUILD_TYPE}" STREQUAL "Release")
        set(CMAKE_OSX_ARCHITECTURES i386 x86_64)
    endif()

    set(CMAKE_OSX_DEPLOYMENT_TARGET 10.5)

    # Set output directories for binaries and static and shared libraries
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

    set(TARGET_NAME_APP_COCOA "AXR Prototype")
    set(TARGET_NAME_APP_SDL AXRSDL)
    set(TARGET_NAME_LIB_CORE AXRCore)
    set(TARGET_NAME_LIB_COCOA AXRCocoa)

    add_subdirectory(src)
endif()

if(BUILD_DOCS)
    add_subdirectory(docs)
endif()
