if(BUILD_SHARED_LIBS)
    add_definitions(-DAXR_EXPORTS)
endif()

# Find all sources
file(GLOB_RECURSE test_SOURCES *.c *.cpp *.h)
list(SORT test_SOURCES)

group_by_folder(${CMAKE_CURRENT_SOURCE_DIR} "${test_SOURCES}")

add_library(TEST ${test_SOURCES})
target_link_libraries(TEST ${TARGET_NAME_LIB_CORE})
set_target_properties(TEST PROPERTIES FOLDER Libraries)

if(APPLE AND BUILD_SHARED_LIBS AND AXR_BUILD_FRAMEWORKS)
    set_target_properties(TEST PROPERTIES FRAMEWORK TRUE)
endif()

if(IS_RELEASE_BUILD)
    install(TARGETS TEST
        LIBRARY DESTINATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY_REL} COMPONENT libraries
        ARCHIVE DESTINATION ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY_REL} COMPONENT libraries
        FRAMEWORK DESTINATION Frameworks COMPONENT frameworks
    )
endif()
