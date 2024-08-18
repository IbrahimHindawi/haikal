cmake_minimum_required(VERSION 3.10)
project(haikal VERSION 1.0 LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src/)
# include_directories(src/bstring/bstring)
add_executable(${PROJECT_NAME}
    src/meta.c
    src/bstring/bstring/bstrlib.c
)
# set(CMAKE_FOLDER include)
