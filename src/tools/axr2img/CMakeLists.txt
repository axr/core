set(axr2img_SOURCES
    main.cpp
)

qt_transform_sources(axr2img_SOURCES "${axr2img_SOURCES}")

add_executable(axr2img ${axr2img_SOURCES})
target_link_libraries(axr2img ${TARGET_NAME_LIB_CORE})

set_target_properties(axr2img PROPERTIES FOLDER Tools)

install(TARGETS axr2img
    RUNTIME DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_REL} COMPONENT tools
)
