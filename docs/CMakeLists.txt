find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile ${CMAKE_BINARY_DIR}/Doxyfile)

    if(FANCY_TARGET_NAMES)
        set(doxygen_target Documentation)
    else()
        set(doxygen_target docs)
    endif()

    add_custom_target(${doxygen_target}
        COMMAND ${DOXYGEN_EXECUTABLE}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Building AXR documentation using Doxygen..."
        SOURCES Doxyfile
    )

    install(DIRECTORY "${CMAKE_BINARY_DIR}/docs/html"
        COMPONENT doc
        DESTINATION "share/doc/axr"
    )
else()
    message(STATUS "Doxygen was NOT found; will not build documentation")
endif()
