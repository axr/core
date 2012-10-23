include(CMakeParseArguments)

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

# The empty.lproj file is needed to translate the strings in native OS X dialogs according to the selected system locale
macro(empty_lproj target)
    if(APPLE)
        install(CODE "file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${target}${CMAKE_USER_EXECUTABLE_SUFFIX}/Contents/Resources/empty.lproj\" \"\")")
    endif()
endmacro()

function(set_msvc_warning_level level)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W${level}" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W${level}")
    endif()
endfunction()

function(set_gcc_warning_level level)
    if(CMAKE_CXX_FLAGS MATCHES "-Wall")
        string(REGEX REPLACE "-Wall" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endif()

    if(CMAKE_CXX_FLAGS MATCHES "-pedantic")
        string(REGEX REPLACE "-pedantic" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endif()

    if(CMAKE_CXX_FLAGS MATCHES "-Wnone")
        string(REGEX REPLACE "-Wnone" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endif()

    if("${level}" STREQUAL "max")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")
    elseif("${level}" STREQUAL "none")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnone")
    endif()
endfunction()

function(set_clang_warning_level level)
    if(CMAKE_CXX_FLAGS MATCHES "-Weverything")
        string(REGEX REPLACE "-Weverything" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endif()

    if("${level}" STREQUAL "max")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything")
    endif()
endfunction()
