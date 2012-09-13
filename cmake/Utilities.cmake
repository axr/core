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
