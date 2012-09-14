# Find all sources for core library
file(GLOB_RECURSE core_SOURCES *.c *.cpp *.h)
list(SORT core_SOURCES)

# Remove Windows-specific sources
file(GLOB_RECURSE windows_SOURCES os/win/*)

if(NOT WIN32)
    list(REMOVE_ITEM core_SOURCES ${windows_SOURCES})
endif()

group_by_folder(${CMAKE_CURRENT_SOURCE_DIR} "${core_SOURCES}")

add_library(${TARGET_NAME_LIB_CORE} ${core_SOURCES})
target_link_libraries(${TARGET_NAME_LIB_CORE} ${Boost_SYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY})
