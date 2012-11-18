set(qt_SOURCES
    QAXRWidget.cpp
    QAXRWidget.h
)

qt_transform_sources(qt_SOURCES "${qt_SOURCES}")

add_library("${TARGET_NAME_LIB_QT}" STATIC ${qt_SOURCES})
set_target_properties("${TARGET_NAME_LIB_QT}" PROPERTIES OUTPUT_NAME axrqt FOLDER Libraries)

# We need not link to Qt directly since core itself does - CMake takes care of that
target_link_libraries("${TARGET_NAME_LIB_QT}" ${TARGET_NAME_LIB_CORE})

install(TARGETS "${TARGET_NAME_LIB_QT}"
    ARCHIVE DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_REL} COMPONENT libraries
)
