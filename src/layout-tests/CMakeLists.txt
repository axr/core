set(layout-tests_SOURCES
	json.h
	json.cpp
    main.cpp
)

qt_transform_sources(layout-tests_SOURCES "${layout-tests_SOURCES}")

add_executable(layout-tests ${layout-tests_SOURCES})
target_link_libraries(layout-tests ${TARGET_NAME_LIB_CORE} ${QT_QTSCRIPT_LIBRARY})

set_target_properties(layout-tests PROPERTIES FOLDER Tools)
