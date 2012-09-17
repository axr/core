cmake_minimum_required(VERSION 2.8)

if("${CMAKE_SOURCE_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "You should not build in-source. Use a directory outside the source tree or a subdirectory such as: ${CMAKE_SOURCE_DIR}/build")
endif()

# Default to debug builds
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.5)

# Set the module path and include utility modules
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_CURRENT_SOURCE_DIR}/cmake/solar ${CMAKE_MODULE_PATH})
include(Utilities)
include(Solar)

# Verify that the source/build paths do not contain any spaces
check_pathnames()

# Compiler detection
check_clang()

project(AXR)

# Help out Windows
use_qt_sdk_locations()

set(AXR_VERSION_STRING "0.4.7")

# This list is for OS X application bundles to copy in all necessary icons
file(GLOB axr_icon_SOURCES ${CMAKE_SOURCE_DIR}/share/icons/*.icns)

# The HSS framework, to be copied into application bundles
file(GLOB hss_SOURCES ${CMAKE_SOURCE_DIR}/share/hss-framework/*)
list(REMOVE_ITEM hss_SOURCES ${CMAKE_SOURCE_DIR}/share/hss-framework/.git)

# If we're using an IDE build system such as Visual Studio or Xcode,
# use fancier target names such as "Documentation" vs "docs"
string(FIND "${CMAKE_GENERATOR}" "Visual Studio" found_vs)
string(FIND "${CMAKE_GENERATOR}" "CodeBlocks" found_blocks)
if (found_vs GREATER -1 OR found_blocks GREATER -1 OR "${CMAKE_GENERATOR}" STREQUAL "Xcode")
    set(FANCY_TARGET_NAMES ON)
endif()

option(AXR_NO_DEPRECATED_ENUMS "Disallow use of deprecated enum members" OFF)
option(AXR_ALL_WARNINGS "Display as many warnings as possible" OFF)
option(AXR_BUILD_PROTOTYPE "Builds the prototype including libraries and demo app" ON)
option(AXR_BUILD_DOCS "Build documentation using doxygen" ON)
option(AXR_BUILD_EXAMPLES "Builds example programs showing usage of the AXR library" OFF)

# Default to building universal binaries on OS X only during release builds
if("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    set(unibin ON)
else()
    set(unibin OFF)
endif()

option(BUILD_UNIVERSAL_BINS "Build OS X universal binaries (i386 + x86_64)" ${unibin})
option(BUILD_SHARED_LIBS "Builds shared instead of static libraries" ON)

if(AXR_BUILD_PROTOTYPE)
    find_package(Qt4 4.8 COMPONENTS QtCore QtGui QtXml REQUIRED)
    include(${QT_USE_FILE})
    add_definitions(${QT_DEFINITIONS})

    # Don't use static Boost because this will result in relocation errors due to
    # Boost not being compiled with -fPIC (at least on 64-bit Linux)
    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)

    find_package(Boost COMPONENTS thread system program_options REQUIRED)
    include_directories(${Boost_INCLUDE_DIRS})

    if(AXR_ALL_WARNINGS)
        if(CMAKE_COMPILER_IS_CLANGXX OR CMAKE_COMPILER_IS_GNUCXX)
            add_definitions(-Wall -pedantic)

            # Treat all external dependency headers as system headers so we don't see warnings for them
            foreach(dir ${Boost_INCLUDE_DIRS} ${SDL_INCLUDE_DIR} ${QT_INCLUDES})
                add_definitions(-isystem ${dir})
            endforeach()
        endif()

        if(CMAKE_COMPILER_IS_CLANGXX)
            add_definitions(-Weverything)
        endif()
    endif()

    if(AXR_NO_DEPRECATED_ENUMS)
        add_definitions(-DAXR_NO_DEPRECATED_ENUMS)
    endif()

    if(BUILD_UNIVERSAL_BINS)
        set(CMAKE_OSX_ARCHITECTURES "i386 x86_64")
    endif()

    # Required to avoid "Symbol not found: __objc_empty_cache" from dyld
    if(APPLE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lobjc")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lobjc")
        set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -lobjc")
    endif()

    # Set output directories for binaries and static and shared libraries
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

    set(TARGET_NAME_APP_COCOA Cocoa)
    set(TARGET_NAME_APP_SDL SDL)
    set(TARGET_NAME_APP_PROTOTYPE Prototype)
    set(TARGET_NAME_LIB_CORE Core)
    set(TARGET_NAME_LIB_COCOA CoreCocoa)

    add_subdirectory(src)
endif()

if(AXR_BUILD_DOCS)
    add_subdirectory(docs)
endif()

if(FANCY_TARGET_NAMES)
    # All of the metadata-type files in the repository root directory
    add_custom_target("Meta" SOURCES AUTHORS CHANGELOG.md COPYING INSTALL LICENSE README.md)

    # Configuration and utility scripts
    file(GLOB_RECURSE scripts_SOURCES .travis/configure.sh .travis/make.sh .travis.yml .gitignore .mailmap bin/pre-commit bin/stylecheck.py bin/update-authors.sh pre-configure.bat pre-configure.sh)
    group_by_folder(${CMAKE_CURRENT_SOURCE_DIR} "${scripts_SOURCES}")
    add_custom_target("Scripts" SOURCES ${scripts_SOURCES})

    # All CMake scripts since some IDEs don't show the hierarchy
    file(GLOB_RECURSE cmake_SOURCES ${CMAKE_SOURCE_DIR}/*/CMakeLists.txt ${CMAKE_SOURCE_DIR}/*.cmake)
    group_by_folder(${CMAKE_CURRENT_SOURCE_DIR} "${cmake_SOURCES}")
    add_custom_target("CMake" SOURCES ${cmake_SOURCES})

    # The HSS framework
    add_custom_target("HSS" SOURCES ${hss_SOURCES})

    # Other shared resources (i.e. to be distributed in $prefix/share or elsewhere)
    set(d ${CMAKE_SOURCE_DIR}/share)
    file(GLOB_RECURSE shared_SOURCES ${d}/*.txt ${d}/*.ico ${d}/*.icns ${d}/*.png)
    list(REMOVE_ITEM shared_SOURCES ${CMAKE_SOURCE_DIR}/share/hss-framework)
    group_by_folder(${d} "${shared_SOURCES}")
    add_custom_target("Shared" SOURCES ${shared_SOURCES})

    # Test files for the renderer
    set(d ${CMAKE_SOURCE_DIR}/tests)
    file(GLOB_RECURSE tests_SOURCES ${d}/*.hss ${d}/*.xml ${d}/*.txt ${d}/*/README)
    group_by_folder(${d} "${tests_SOURCES}")
    add_custom_target("Tests" SOURCES ${tests_SOURCES})
endif()
