set(cocoa_SOURCES
    NSAXRDocument.h
    NSAXRDocument.mm
    NSAXRView.h
    NSAXRView.mm
)

qt_transform_sources(cocoa_SOURCES "${cocoa_SOURCES}")

add_library("${TARGET_NAME_LIB_COCOA}" STATIC ${cocoa_SOURCES})
set_target_properties("${TARGET_NAME_LIB_COCOA}" PROPERTIES OUTPUT_NAME axrcocoa FOLDER Libraries)

find_library(COCOA_LIBRARY Cocoa)
mark_as_advanced(COCOA_LIBRARY)
target_link_libraries("${TARGET_NAME_LIB_COCOA}" ${COCOA_LIBRARY} ${TARGET_NAME_LIB_CORE})

install(TARGETS "${TARGET_NAME_LIB_COCOA}"
    ARCHIVE DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_REL} COMPONENT libraries
)
