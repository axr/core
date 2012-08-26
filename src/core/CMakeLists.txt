file(GLOB_RECURSE core_SOURCES *.c *.cpp *.h *.m *.mm)
list(SORT core_SOURCES)

if(NOT WIN32)
    file(GLOB_RECURSE windows_SOURCES os/win/*)
    list(REMOVE_ITEM core_SOURCES ${windows_SOURCES})
endif()

if(NOT APPLE)
    file(GLOB_RECURSE osx_SOURCES os/osx/*)
    list(REMOVE_ITEM core_SOURCES ${osx_SOURCES})
endif()

foreach(core_SOURCE ${core_SOURCES})
    # Get the path of the file relative to the current source directory
    file(RELATIVE_PATH core_SOURCE_relative "${CMAKE_CURRENT_SOURCE_DIR}" "${core_SOURCE}")

    # Get the relative folder path
    get_filename_component(core_SOURCE_dir "${core_SOURCE_relative}" PATH)

    # Convert forward slashes to backslashes to get source group identifiers
    string(REPLACE "/" "\\" core_SOURCE_group "${core_SOURCE_dir}")

    source_group("${core_SOURCE_group}" FILES "${core_SOURCE}")
endforeach()

add_library(AXR_core ${core_SOURCES})

if(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    mark_as_advanced(COCOA_LIBRARY)
    target_link_libraries(AXR_core ${COCOA_LIBRARY})
endif()

target_link_libraries(AXR_core ${Boost_LIBRARIES} ${EXPAT_LIBRARIES} ${CAIRO_LIBRARIES} ${PANGO_LIBRARIES} ${PANGOCAIRO_LIBRARIES})
