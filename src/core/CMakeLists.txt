if(BUILD_SHARED_LIBS)
    add_definitions(-DAXR_EXPORTS)
endif()

# Find all sources for core library
file(GLOB_RECURSE core_SOURCES *.c *.cpp *.h)
list(SORT core_SOURCES)

group_by_folder(${CMAKE_CURRENT_SOURCE_DIR} "${core_SOURCES}")

add_library(${TARGET_NAME_LIB_CORE} ${core_SOURCES})
target_link_libraries(${TARGET_NAME_LIB_CORE} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY})

if(IS_RELEASE_BUILD)
    install(TARGETS ${TARGET_NAME_LIB_CORE}
        LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_REL} COMPONENT libraries
        ARCHIVE DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_REL} COMPONENT libraries
        FRAMEWORK DESTINATION Frameworks COMPONENT frameworks
    )
endif()
