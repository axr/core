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

# Adds a tree of directories to include_directories, recursively
macro(include_directories_recursive root_header_dir)
    file(GLOB_RECURSE new_list "${root_header_dir}/*.h")
    set(dir_list "")
    foreach(file_path ${new_list})
        get_filename_component(dir_path ${file_path} PATH)
        set(dir_list ${dir_list} ${dir_path})
    endforeach()
    list(REMOVE_DUPLICATES dir_list)
    include_directories(${dir_list})
endmacro()

# The empty.lproj file is needed to translate the strings in native OS X dialogs according to the selected system locale
macro(empty_lproj target comp)
    if(APPLE)
        install(CODE "file(WRITE \"\${CMAKE_INSTALL_PREFIX}/${target}${CMAKE_USER_EXECUTABLE_SUFFIX}/Contents/Resources/empty.lproj\" \"\")" COMPONENT ${comp})
    endif()
endmacro()

function(set_msvc_warning_level level)
    if(CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
        string(REGEX REPLACE "/W[0-4]" "/W${level}" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" PARENT_SCOPE)
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W${level}" PARENT_SCOPE)
    endif()
endfunction()

function(set_gcc_warning_level level)
    if(CMAKE_CXX_FLAGS MATCHES "-Wall")
        string(REGEX REPLACE "-Wall" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endif()

    if(CMAKE_CXX_FLAGS MATCHES "-pedantic")
        string(REGEX REPLACE "-pedantic" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endif()

    if(CMAKE_CXX_FLAGS MATCHES "-w")
        string(REGEX REPLACE "-w" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endif()

    if("${level}" STREQUAL "max")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic" PARENT_SCOPE)
    elseif("${level}" STREQUAL "none")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -w" PARENT_SCOPE)
    endif()
endfunction()

function(set_clang_warning_level level)
    if(CMAKE_CXX_FLAGS MATCHES "-Weverything")
        string(REGEX REPLACE "-Weverything" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    endif()

    if("${level}" STREQUAL "max")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything" PARENT_SCOPE)
    endif()

    if("${level}" STREQUAL "sane")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Weverything -Wno-padded -Wno-switch-enum" PARENT_SCOPE)
    endif()
endfunction()
