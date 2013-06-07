set(layout-tests_SOURCES
	json.h
	json.cpp
    main.cpp
)

qt_transform_sources(layout-tests_SOURCES "${layout-tests_SOURCES}")

add_executable(layout-tests ${layout-tests_SOURCES})
target_link_libraries(layout-tests ${TARGET_NAME_LIB_CORE} ${QT_QTSCRIPT_LIBRARY})

set_target_properties(layout-tests PROPERTIES FOLDER Tools)

# Temporarily disabled on Windows
if(NOT WIN32)
    get_property(LAYOUT_TESTS_BIN TARGET layout-tests PROPERTY LOCATION)
    get_filename_component(LAYOUT_TESTS_PATH "${LAYOUT_TESTS_BIN}" PATH)

    add_custom_target(run-layout-tests ALL
        COMMAND "${LAYOUT_TESTS_BIN}" "${AXR_CORE_SOURCE_DIR}/tests/layoutTests/tests.json" --format xml --stylesheet "${AXR_CORE_SOURCE_DIR}/tests/layoutTests/style.hss"
        DEPENDS layout-tests
        WORKING_DIRECTORY "${LAYOUT_TESTS_PATH}"
    )

    set_target_properties(run-layout-tests PROPERTIES FOLDER Tools)
endif()
