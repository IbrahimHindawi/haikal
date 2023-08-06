cmake_minimum_required(VERSION 3.10)

project(haikal VERSION 1.0)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_executable(${PROJECT_NAME}
    source/main.c
    source/hkArray.c
)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED True)

include_directories(include/)
set(CMAKE_FOLDER include)
