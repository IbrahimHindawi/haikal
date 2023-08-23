cmake_minimum_required(VERSION 3.10)

project(haikal VERSION 1.0 LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_library(${PROJECT_NAME}
    source/hkArray.c
    include/hkArray.h
)

target_include_directories(${PROJECT_NAME} PUBLIC include/)

# target_link_libraries(haikal PUBLIC )

# add_executable(${PROJECT_NAME}
#     source/main.c
#     source/hkArray.c
# )
# 
# include_directories(include/)
# set(CMAKE_FOLDER include)

# set(CMAKE_C_STANDARD 99)
# set(CMAKE_C_STANDARD_REQUIRED True)

