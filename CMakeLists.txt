cmake_minimum_required(VERSION 3.10)
project(haikal VERSION 1.0 LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src/)
include_directories(src/runtime)
include_directories(src/meta_arena/gen)
include_directories(extern/tree-sitter/lib/include)
include_directories(extern/tree-sitter/lib/src)
include_directories(extern/tree-sitter-c/src)
# include_directories(src/bstring/bstring)
set_source_files_properties(
    extern/tree-sitter-c/src/parser.c
    PROPERTIES COMPILE_OPTIONS "-Wno-unused-but-set-variable"
)
add_executable(${PROJECT_NAME}
    src/main.c
    src/bstring/bstring/bstrlib.c
    extern/tree-sitter/lib/src/lib.c
    extern/tree-sitter-c/src/parser.c
)

add_executable(haikal_test EXCLUDE_FROM_ALL
    src/test.c
    src/tests.c
    src/bstring/bstring/bstrlib.c
)
# set(CMAKE_FOLDER include)
