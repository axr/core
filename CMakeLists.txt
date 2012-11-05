if(WIN32 OR APPLE)
    cmake_minimum_required(VERSION 2.8.7)
else()
    cmake_minimum_required(VERSION 2.8)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake ${CMAKE_CURRENT_SOURCE_DIR}/cmake/solar ${CMAKE_MODULE_PATH})
include(ForceOutBuild)
include(ForceLatinPaths)

project(AXRCore)

if("${CMAKE_BUILD_TYPE}" STREQUAL Release)
    set(IS_RELEASE_BUILD TRUE)
    set(IS_DEBUG_BUILD FALSE)
else()
    # Default to debug builds
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug)
    endif()

    set(IS_RELEASE_BUILD FALSE)
    set(IS_DEBUG_BUILD TRUE)
endif()

set(CMAKE_OSX_DEPLOYMENT_TARGET 10.5)

# Options
option(AXR_NO_DEPRECATED_ENUMS "Disallow use of deprecated enum members" OFF)
option(AXR_ALL_WARNINGS "Display as many warnings as possible" OFF)
option(AXR_BUILD_CORE "Builds the core library" ON)
option(AXR_BUILD_DOCS "Build documentation using doxygen" OFF)
option(AXR_BUILD_FRAMEWORKS "Build AXR libraries as frameworks on OS X" ON)
option(BUILD_UNIVERSAL_BINS "Build OS X universal binaries (i386 + x86_64)" ${IS_RELEASE_BUILD})
option(BUILD_SHARED_LIBS "Builds shared instead of static libraries" ON)

set(AXR_BUILD_EXTENSIONS "" CACHE STRING "Builds extension libraries for toolkit integration")

if(BUILD_UNIVERSAL_BINS)
    set(CMAKE_OSX_ARCHITECTURES "i386;x86_64")
endif()

# Include utility modules
include(Utilities)
include(Solar)
include(FindDependencies)
include(Variables)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    include(Package)
    include(DeployQt4)
endif()

# This list is for OS X application bundles to copy in all necessary icons
file(GLOB axr_icon_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/share/icons/*.icns)

# The HSS framework, to be copied into application bundles
file(GLOB hss_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/share/hss-framework/*)
list(REMOVE_ITEM hss_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/share/hss-framework/.git)

# If we're using an IDE build system such as Visual Studio or Xcode,
# use fancier target names such as "Documentation" vs "docs"
string(FIND "${CMAKE_GENERATOR}" "Visual Studio" found_vs)
string(FIND "${CMAKE_GENERATOR}" "CodeBlocks" found_blocks)
if (found_vs GREATER -1 OR found_blocks GREATER -1 OR "${CMAKE_GENERATOR}" STREQUAL "Xcode")
    set(FANCY_TARGET_NAMES ON)
endif()

if(AXR_BUILD_CORE)
    # Treat all external dependency headers as system headers so we don't see warnings for them
    include_directories(SYSTEM ${QT_INCLUDES})

    if(AXR_ALL_WARNINGS)
        if(MSVC)
            set_msvc_warning_level(4)
        endif()

        if(CMAKE_COMPILER_IS_CLANGXX OR CMAKE_COMPILER_IS_GNUCXX)
            set_gcc_warning_level("max")
        endif()

        if(CMAKE_COMPILER_IS_CLANGXX)
            set_clang_warning_level("max")
        endif()
    endif()

    if(AXR_NO_DEPRECATED_ENUMS)
        add_definitions(-DAXR_NO_DEPRECATED_ENUMS)
    endif()

    add_definitions(-DQT_NO_URL_CAST_FROM_STRING)

    # Required to avoid "Symbol not found: __objc_empty_cache" from dyld
    if(APPLE)
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -lobjc")
        set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -lobjc")
        set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -lobjc")
    endif()

    if(UNIX)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_REL bin)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_REL lib)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_REL lib)
    else()
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_REL .)
        set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_REL .)
        set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_REL .)
        set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)
    endif()

    set(CPACK_OUTPUT_FILE_PREFIX dist)

    # Set output directories for binaries and static and shared libraries
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY_REL})
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_REL})
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_LIBRARY_OUTPUT_DIRECTORY_REL})

    list(APPEND ALL_LIBRARY_DIRS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
    list(APPEND ALL_LIBRARY_DIRS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}")
    list(APPEND ALL_LIBRARY_DIRS "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    list(APPEND ALL_LIBRARY_DIRS "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}")

    if(FANCY_TARGET_NAMES)
        set(TARGET_NAME_LIB_CORE Core)
        set(TARGET_NAME_LIB_QT "Qt integration")
        set(TARGET_NAME_LIB_COCOA "Cocoa integration")
        set(TARGET_NAME_APP_BROWSER Browser)
        set(TARGET_NAME_APP_COCOA "Cocoa example")
        set(TARGET_NAME_APP_SDL "SDL example")
    else()
        set(TARGET_NAME_LIB_CORE core)
        set(TARGET_NAME_LIB_QT qt)
        set(TARGET_NAME_LIB_COCOA cocoa)
        set(TARGET_NAME_APP_BROWSER browser)
        set(TARGET_NAME_APP_COCOA cocoa_example)
        set(TARGET_NAME_APP_SDL sdl_example)
    endif()

    configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.in.h" "${CMAKE_BINARY_DIR}/axr-config.h")

    add_subdirectory(src)
endif()

if(AXR_BUILD_DOCS)
    if(FANCY_TARGET_NAMES)
        set(TARGET_NAME_DOCUMENTATION Documentation)
    else()
        set(TARGET_NAME_DOCUMENTATION docs)
    endif()

    add_subdirectory(docs)
endif()

if(FANCY_TARGET_NAMES)
    # All of the metadata-type files in the repository root directory
    add_custom_target("Meta" SOURCES AUTHORS CHANGELOG.md LICENSE README.md)
    set_target_properties(Meta PROPERTIES FOLDER Auxiliary)

    # Configuration and utility scripts
    file(GLOB_RECURSE scripts_SOURCES .travis/configure.sh .travis/make.sh .travis.yml .gitignore .mailmap bin/pre-commit bin/stylecheck.py bin/update-authors.sh pre-configure.bat pre-configure.sh)
    group_by_folder(${CMAKE_CURRENT_SOURCE_DIR} "${scripts_SOURCES}")
    add_custom_target("Scripts" SOURCES ${scripts_SOURCES})
    set_target_properties(Scripts PROPERTIES FOLDER Auxiliary)

    # All CMake scripts since some IDEs don't show the hierarchy
    file(GLOB_RECURSE cmake_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*/CMakeLists.txt ${CMAKE_CURRENT_SOURCE_DIR}/*.cmake)
    group_by_folder(${CMAKE_CURRENT_SOURCE_DIR} "${cmake_SOURCES}")
    add_custom_target("CMake" SOURCES ${cmake_SOURCES} config.in.h cmake/PackageOverrides.cmake.in)

    # The HSS framework
    add_custom_target("HSS" SOURCES ${hss_SOURCES})
    set_target_properties(HSS PROPERTIES FOLDER Auxiliary)

    # Other shared resources (i.e. to be distributed in $prefix/share or elsewhere)
    set(d ${CMAKE_CURRENT_SOURCE_DIR}/share)
    file(GLOB_RECURSE shared_SOURCES ${d}/*.txt ${d}/*.ico ${d}/*.icns ${d}/*.png)
    list(REMOVE_ITEM shared_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/share/hss-framework)
    group_by_folder(${d} "${shared_SOURCES}")
    add_custom_target("Shared" SOURCES ${shared_SOURCES})
    set_target_properties(Shared PROPERTIES FOLDER Auxiliary)

    # Test files for the renderer
    set(d ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    file(GLOB_RECURSE tests_SOURCES ${d}/*.hss ${d}/*.xml ${d}/*.txt ${d}/*/README)
    group_by_folder(${d} "${tests_SOURCES}")
    add_custom_target("Tests" SOURCES ${tests_SOURCES})
endif()

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    if(IS_RELEASE_BUILD)
        if(AXR_BUILD_CORE)
            include(InstallRequiredSystemLibraries)
            include(CPack)
        endif()
    else()
        install(CODE "message(FATAL_ERROR \"You cannot install or package debug builds\")")
    endif()
endif()
