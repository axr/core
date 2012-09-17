set(cocoa_SOURCES
    AXRView.h
    AXRView.mm
    CocoaAXRWrapper.h
    CocoaAXRWrapper.mm
)

qt_transform_sources(cocoa_SOURCES "${cocoa_SOURCES}")

add_library(${TARGET_NAME_LIB_COCOA} ${cocoa_SOURCES})

find_library(COCOA_LIBRARY Cocoa)
mark_as_advanced(COCOA_LIBRARY)
target_link_libraries(${TARGET_NAME_LIB_COCOA} ${COCOA_LIBRARY} ${TARGET_NAME_LIB_CORE})

install(TARGETS ${TARGET_NAME_LIB_COCOA}
    LIBRARY DESTINATION lib COMPONENT libraries
    FRAMEWORK DESTINATION Frameworks COMPONENT frameworks
)
