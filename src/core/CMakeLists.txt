if(BUILD_SHARED_LIBS)
    add_definitions(-DAXR_EXPORTS)
endif()

# Find all sources for core library
set(core_SOURCES_PATTERN *.c *.cpp *.h)
if(APPLE)
    set(core_SOURCES_PATTERN ${core_SOURCES_PATTERN} *.mm)
endif()

file(GLOB_RECURSE core_SOURCES ${core_SOURCES_PATTERN})
list(SORT core_SOURCES)

group_by_folder(${CMAKE_CURRENT_SOURCE_DIR} "${core_SOURCES}")

add_library(${TARGET_NAME_LIB_CORE} ${core_SOURCES})
target_link_libraries(${TARGET_NAME_LIB_CORE} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY})

file(GLOB_RECURSE core_HEADERS *.h)
set_target_properties(${TARGET_NAME_LIB_CORE} PROPERTIES
    FOLDER Libraries
    PUBLIC_HEADER "${core_HEADERS}"
)

if(APPLE AND BUILD_SHARED_LIBS AND AXR_BUILD_FRAMEWORKS)
    set_target_properties(${TARGET_NAME_LIB_CORE} PROPERTIES FRAMEWORK TRUE)
endif()

if(APPLE AND AXR_BUILD_FRAMEWORKS)
    set_target_properties(${TARGET_NAME_LIB_CORE} PROPERTIES OUTPUT_NAME AXRCore)
else()
    set_target_properties(${TARGET_NAME_LIB_CORE} PROPERTIES OUTPUT_NAME axrcore)
endif()

# Need to use ../Library trick because absolute paths won't work with CPack
install(TARGETS ${TARGET_NAME_LIB_CORE}
    RUNTIME DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_REL} COMPONENT libraries
    LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_REL} COMPONENT libraries
    ARCHIVE DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_REL} COMPONENT libraries
    FRAMEWORK DESTINATION ../Library/Frameworks COMPONENT libraries
    PUBLIC_HEADER DESTINATION include COMPONENT headers
)
