configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${CMAKE_BINARY_DIR}/Doxyfile)

file(GLOB_RECURSE docs_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/*)
list(APPEND docs_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
group_by_folder(${CMAKE_CURRENT_SOURCE_DIR} "${docs_SOURCES}")

add_custom_target(${TARGET_NAME_DOCUMENTATION} ALL
    COMMAND ${DOXYGEN_EXECUTABLE}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Building AXR documentation using Doxygen..."
    SOURCES ${docs_SOURCES}
)

if(IS_RELEASE_BUILD)
    install(DIRECTORY "${CMAKE_BINARY_DIR}/docs/html"
        COMPONENT doc
        DESTINATION "share/doc/axr"
    )
endif()
