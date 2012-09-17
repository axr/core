include(CMakeParseArguments)

# NOTE: Untested on Windows
function(check_pathnames)
    if(WIN32)
        set(separator ":")
    endif()

    set(path_regex "^([A-Za-z0-9._/-${separator}]+)$")

    if(NOT "${CMAKE_SOURCE_DIR}" MATCHES "${path_regex}" OR NOT "${CMAKE_BINARY_DIR}" MATCHES "${path_regex}")
        message(FATAL_ERROR "Ensure that the source and build paths contain only the following characters: alphanumeric, dash, underscore, slash, dot (and colon on Windows)")
    endif()
endfunction()

function(join_path output_variable components)
    set(final_path .)
    foreach(component ${components})
        set(final_path "${final_path}/${component}")
    endforeach()

    while("${final_path}" MATCHES "^\\./")
        string(SUBSTRING "${final_path}" 2 -1 final_path)
    endwhile()

    set(${output_variable} "${final_path}" PARENT_SCOPE)
endfunction()

function(install_bundle)
    set(options )
    set(oneValueArgs TARGET COMPONENT BUNDLE_DESTINATION RUNTIME_DESTINATION)
    set(multiValueArgs )
    cmake_parse_arguments(install_bundle "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN} )

    if(NOT install_bundle_TARGET)
        message(FATAL_ERROR "install_bundle missing TARGET argument")
    endif()

    if(NOT install_bundle_COMPONENT)
        message(FATAL_ERROR "install_bundle missing COMPONENT argument")
    endif()

    if(NOT install_bundle_BUNDLE_DESTINATION)
        message(FATAL_ERROR "install_bundle missing BUNDLE_DESTINATION argument")
    endif()

    if(NOT install_bundle_RUNTIME_DESTINATION)
        message(FATAL_ERROR "install_bundle missing RUNTIME_DESTINATION argument")
    endif()

    if(APPLE)
        join_path(path_bundle     "${install_bundle_BUNDLE_DESTINATION};${install_bundle_TARGET}.app")
        join_path(path_plugin_dir "${install_bundle_BUNDLE_DESTINATION};${install_bundle_TARGET}.app/Contents/MacOS")
        join_path(path_qtconf_dir "${install_bundle_BUNDLE_DESTINATION};${install_bundle_TARGET}.app/Contents/MacOS")
    else()
        join_path(path_bundle     "${install_bundle_RUNTIME_DESTINATION};${install_bundle_TARGET}${CMAKE_EXECUTABLE_SUFFIX}")
        join_path(path_plugin_dir "${install_bundle_RUNTIME_DESTINATION}")
        join_path(path_qtconf_dir "${install_bundle_RUNTIME_DESTINATION}")
    endif()

    install(TARGETS ${install_bundle_TARGET}
        BUNDLE DESTINATION ${install_bundle_BUNDLE_DESTINATION} COMPONENT ${install_bundle_COMPONENT}
        RUNTIME DESTINATION ${install_bundle_RUNTIME_DESTINATION} COMPONENT ${install_bundle_COMPONENT}
    )

    # Install Qt plugins - can be culled using 'REGEX "..." EXCLUDE'
    # TODO: Install plugins according to which Qt libraries are linked to the target
    install(DIRECTORY "${QT_PLUGINS_DIR}/imageformats" DESTINATION "${path_plugin_dir}/plugins" COMPONENT ${install_bundle_COMPONENT})

    list(APPEND LIBDIRS "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")
    list(APPEND LIBDIRS "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/${CMAKE_BUILD_TYPE}")

    # Make the application bundle standalone
    file(WRITE "${CMAKE_BINARY_DIR}/${install_bundle_TARGET}.Target.CPack.txt" "
        file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${path_qtconf_dir}/qt.conf\" \"\")
        file(GLOB_RECURSE QTPLUGINS \"\${CMAKE_INSTALL_PREFIX}/${path_plugin_dir}/plugins/*${CMAKE_SHARED_LIBRARY_SUFFIX}\")
        include(BundleUtilities)
        fixup_bundle(\"\${CMAKE_INSTALL_PREFIX}/${path_bundle}\" \"\${QTPLUGINS}\" \"${LIBDIRS}\")
        ")

    install(SCRIPT "${CMAKE_BINARY_DIR}/${install_bundle_TARGET}.Target.CPack.txt" COMPONENT ${install_bundle_COMPONENT})
endfunction()
