# Detects whether a preprocessor symbol is defined by the current C compiler
function(is_symbol_defined output_variable symbol)
    enable_language(C)

set(is_symbol_defined_code "
#if defined(${symbol})
int main() { return 0; }
#endif
")

    file(WRITE "${CMAKE_BINARY_DIR}/is_symbol_defined.c" "${is_symbol_defined_code}")

    try_compile(is_symbol_defined_result "${CMAKE_BINARY_DIR}" "${CMAKE_BINARY_DIR}/is_symbol_defined.c")

    if(is_symbol_defined_result)
        set(${output_variable} TRUE PARENT_SCOPE)
    else()
        set(${output_variable} FALSE PARENT_SCOPE)
    endif()
endfunction()

# Checks whether the compiler in use is Clang by asking the compiler itself
# CMAKE_COMPILER_IS_CLANGXX will be set in the parent scope if this is the case
function(check_clang)
    is_symbol_defined(clang_result __clang__)

    if(clang_result)
        set(CMAKE_COMPILER_IS_CLANGXX TRUE PARENT_SCOPE)
    endif()
endfunction()

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
