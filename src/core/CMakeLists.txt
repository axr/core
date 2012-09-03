# Find all sources for core library
file(GLOB_RECURSE core_SOURCES *.c *.cpp *.h)
list(SORT core_SOURCES)

# Remove Windows-specific sources
file(GLOB_RECURSE windows_SOURCES os/win/*)

if(NOT WIN32)
    list(REMOVE_ITEM core_SOURCES ${windows_SOURCES})
endif()

# Sort and group source files
foreach(core_SOURCE ${core_SOURCES})
    # Get the path of the file relative to the current source directory
    file(RELATIVE_PATH core_SOURCE_relative "${CMAKE_CURRENT_SOURCE_DIR}" "${core_SOURCE}")

    # Get the relative folder path
    get_filename_component(core_SOURCE_dir "${core_SOURCE_relative}" PATH)

    # Convert forward slashes to backslashes to get source group identifiers
    string(REPLACE "/" "\\" core_SOURCE_group "${core_SOURCE_dir}")

    source_group("${core_SOURCE_group}" FILES "${core_SOURCE}")
endforeach()

add_library(${TARGET_NAME_LIB_CORE} ${core_SOURCES})
target_link_libraries(${TARGET_NAME_LIB_CORE} ${Boost_LIBRARIES} ${EXPAT_LIBRARIES} ${CAIRO_LIBRARIES} ${PANGO_LIBRARIES} ${PANGOCAIRO_LIBRARIES})

# AXR core documentation
find_package(Doxygen)
if(DOXYGEN_FOUND)
    # Copy the Doxygen configuration file to the output and append the INPUT configuration
    # directive to point to the source directory - we do this instead of using the @ syntax
    # within the file so that Doxygen can be run without requiring CMake to be run
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${CMAKE_BINARY_DIR}/Doxyfile)
    file(APPEND ${CMAKE_BINARY_DIR}/Doxyfile "INPUT = ${CMAKE_CURRENT_SOURCE_DIR}")

    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Building documentation for AXR core..."
        SOURCES Doxyfile
    )
else()
    message(STATUS "Doxygen was NOT found; will not build documentation")
endif()
