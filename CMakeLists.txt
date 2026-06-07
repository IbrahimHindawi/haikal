cmake_minimum_required(VERSION 3.10)
project(haikal VERSION 1.0 LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_path(LIBCLANG_INCLUDE_DIR clang-c/Index.h
    PATHS
        "$ENV{LLVM_ROOT}/include"
        "C:/Program Files/LLVM/include"
)
find_library(LIBCLANG_LIBRARY libclang
    PATHS
        "$ENV{LLVM_ROOT}/lib"
        "C:/Program Files/LLVM/lib"
)

if(NOT LIBCLANG_INCLUDE_DIR OR NOT LIBCLANG_LIBRARY)
    message(FATAL_ERROR "libclang development files not found. Set LLVM_ROOT to the LLVM install root.")
endif()

include_directories(src/)
include_directories(src/runtime)
include_directories(src/meta_arena/gen)
include_directories(${LIBCLANG_INCLUDE_DIR})
# include_directories(src/bstring/bstring)
add_executable(${PROJECT_NAME}
    src/main.c
    src/bstring/bstring/bstrlib.c
)
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBCLANG_LIBRARY})

add_executable(haikal_test EXCLUDE_FROM_ALL
    src/test.c
    src/tests.c
    src/bstring/bstring/bstrlib.c
)
# set(CMAKE_FOLDER include)
