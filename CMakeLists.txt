cmake_minimum_required(VERSION 3.10)
project(haikal VERSION 1.0 LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME}
    src/meta.c
    bstring/bstring/bstrlib.c
)
include_directories(src/)
include_directories(bstring/bstring)
# set(CMAKE_FOLDER include)

add_executable(test
    src/test.c
)
include_directories(src/meta/gen)
# add_library(${PROJECT_NAME}
#     source/hkArray.c
#     include/hkArray.h
# )
# target_include_directories(${PROJECT_NAME} PUBLIC include/)
# target_link_libraries(haikal PUBLIC )

