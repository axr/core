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
