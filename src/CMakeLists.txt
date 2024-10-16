configure_file("config.h.in" "config.h")

add_custom_command(
    OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c"
    COMMAND "${YACC}" "-ydtv" "${CMAKE_CURRENT_SOURCE_DIR}/grammar.y"
    DEPENDS "grammar.y"
)

add_custom_command(
    OUTPUT "lex.yy.c"
    COMMAND "${FLEX}" "--header-file=lex.yy.h" "${CMAKE_CURRENT_SOURCE_DIR}/lexer.l"
    BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/lex.yy.h"
    DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c" "lexer.l"
)

add_executable(flathead
    "${CMAKE_CURRENT_BINARY_DIR}/y.tab.c" "${CMAKE_CURRENT_BINARY_DIR}/lex.yy.c"
    "eval.c" "str.c" "regexp.c" "cli.c"
    "nodes.c" "args.c" "flathead.c" "debug.c" "gc.c" "props.c"
    "runtime/runtime.c" "runtime/lib/Math.c" "runtime/lib/RegExp.c"
    "runtime/lib/Error.c" "runtime/lib/String.c" "runtime/lib/console.c"
    "runtime/lib/gc.c" "runtime/lib/Function.c" "runtime/lib/Object.c"
    "runtime/lib/Boolean.c" "runtime/lib/Number.c" "runtime/lib/Date.c"
    "runtime/lib/Array.c"
)
target_include_directories(flathead PRIVATE "${CMAKE_CURRENT_BINARY_DIR}" "${PROJECT_SOURCE_DIR}")
set_target_properties(flathead PROPERTIES OUTPUT_NAME "flat")

target_link_libraries(flathead PRIVATE ${${FH_LINE_EDITOR}_LIBRARIES})
target_include_directories(flathead PRIVATE ${${FH_LINE_EDITOR}_INCLUDE_DIRS})

if(FH_REGEXP)
    target_link_libraries(flathead PRIVATE ${${FH_REGEXP_IMPL}_LIBRARIES})
    target_link_libraries(flathead PRIVATE ${${FH_REGEXP_IMPL}_INCLUDE_DIRS})
endif()
