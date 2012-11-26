if(BUILD_SHARED_LIBS)
    add_definitions(-DAXR_EXPORTS)
endif()

# Find all sources
file(GLOB_RECURSE test_SOURCES *.c *.cpp *.h)
list(SORT test_SOURCES)

group_by_folder(${CMAKE_CURRENT_SOURCE_DIR} "${test_SOURCES}")

# Static-only build to ease Windows development
# since this shouldn't really even be here anyways
add_library(TEST STATIC ${test_SOURCES})
target_link_libraries(TEST ${TARGET_NAME_LIB_CORE})
set_target_properties(TEST PROPERTIES FOLDER Libraries)
