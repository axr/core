include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/core"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/axr"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/axr/errors"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/objects"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/parsing"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/tokenizing"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/various"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/os"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/os/win"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/xml"
)

add_subdirectory(core)

if(APPLE)
    include_directories("${CMAKE_CURRENT_SOURCE_DIR}/cocoa")
    add_subdirectory(cocoa)
endif()

add_subdirectory(prototype)

if(AXR_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
