find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${CMAKE_BINARY_DIR}/Doxyfile)

    add_custom_target(${TARGET_NAME_DOCUMENTATION} ALL
        COMMAND ${DOXYGEN_EXECUTABLE}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Building AXR documentation using Doxygen..."
        SOURCES Doxyfile
    )

    if(IS_RELEASE_BUILD)
        install(DIRECTORY "${CMAKE_BINARY_DIR}/docs/html"
            COMPONENT doc
            DESTINATION "share/doc/axr"
        )
    endif()
else()
    message(STATUS "Doxygen was NOT found; will not build documentation")
endif()
