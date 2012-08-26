file(GLOB AXR_CORE_SRCS *.cpp *.h)
file(GLOB_RECURSE AXR_SRCS axr/*.cpp axr/*.h)
file(GLOB_RECURSE XML_SRCS xml/*.cpp xml/*.h)
file(GLOB_RECURSE HSS_SRCS hss/*.cpp hss/*.h)

if(WIN32)
    file(GLOB_RECURSE WRAPPER_SRCS os/win/*.cpp os/win/*.h)
elseif(APPLE)
    file(GLOB_RECURSE WRAPPER_SRCS os/osx/*.mm os/osx/*.h)
endif()

file(GLOB WRAPPER_SRCS_GENERIC os/*.cpp os/*.h)

source_group("AXR" FILES ${AXR_SRCS})
source_group("XML" FILES ${XML_SRCS})
source_group("HSS" FILES ${HSS_SRCS})
source_group("OS" FILES ${WRAPPER_SRCS} ${WRAPPER_SRCS_GENERIC})
source_group("" FILES ${AXR_CORE_SRCS})

add_library(AXR_core ${AXR_CORE_SRCS} ${AXR_SRCS} ${XML_SRCS} ${HSS_SRCS} ${WRAPPER_SRCS} ${WRAPPER_SRCS_GENERIC})

if(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    mark_as_advanced(COCOA_LIBRARY)
    target_link_libraries(AXR_core ${COCOA_LIBRARY})
endif()

target_link_libraries(AXR_core ${Boost_LIBRARIES} ${EXPAT_LIBRARIES} ${CAIRO_LIBRARIES} ${PANGO_LIBRARIES} ${PANGOCAIRO_LIBRARIES})
