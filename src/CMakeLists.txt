include_directories_recursive("${CMAKE_CURRENT_SOURCE_DIR}/core")
add_subdirectory(core)

if(AXR_BUILD_EXTENSIONS MATCHES "QT")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/qt")
    add_subdirectory(qt)
endif()

if(APPLE AND AXR_BUILD_EXTENSIONS MATCHES "COCOA")
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/cocoa")
    add_subdirectory(cocoa)
endif()

# TODO: Remove this and integrate these files' functionality into unit tests
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/TEST")
add_subdirectory(TEST)
