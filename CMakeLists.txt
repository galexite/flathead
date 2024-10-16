cmake_minimum_required(VERSION 3.28)

project(flathead C)

find_program(YACC yacc REQUIRED)
message(STATUS "Found yacc: ${YACC}")
find_program(FLEX flex REQUIRED)
message(STATUS "Found flex: ${FLEX}")

find_package(PkgConfig)
pkg_check_modules(readline readline)
set(FH_LINE_EDITOR "")
if(readline_FOUND)
    set(FH_HAVE_READLINE TRUE)
    set(FH_LINE_EDITOR "readline")
else()
    pkg_check_modules(editline libedit)
    if(editline_FOUND)
        set(FH_HAVE_EDITLINE TRUE)
        set(FH_LINE_EDITOR "editline")
    else()
        message(FATAL_ERROR "No line editor library found (readline or editline required).")
    endif()
endif()

pkg_check_modules(libpcre2 libpcre2-8)
set(FH_REGEXP_IMPL "")
if (libpcre2_FOUND)
    set(FH_HAVE_PCRE2 TRUE)
    set(FH_REGEXP_IMPL "pcre2")
endif()

include(CMakeDependentOption)

cmake_dependent_option(FH_REGEXP ON "Enable regular expressions (requires pcre2)" "FH_REGEXP_IMPL" OFF)

add_subdirectory(src)
