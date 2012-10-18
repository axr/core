include_directories(
    "${CMAKE_CURRENT_SOURCE_DIR}/core"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/axr"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/debug"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/objects"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/objects/actions"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/objects/borders"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/objects/gradients"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/objects/shapes"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/parsing"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/parsing/constants"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/parsing/expressions"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/parsing/filters"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/parsing/functions"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/parsing/selectors"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/parsing/statements"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/tokenizing"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/hss/various"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/rendering"
    "${CMAKE_CURRENT_SOURCE_DIR}/core/types"
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

if(AXR_BUILD_PLUGIN)
    set(FB_PROJECTS_DIR "${CMAKE_SOURCE_DIR}/src")
    add_definitions(-DQT_NO_KEYWORDS)
    add_subdirectory(firebreath)
endif()
