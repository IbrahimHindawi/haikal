cmake_minimum_required(VERSION 3.10)
project(haikal VERSION 1.0 LANGUAGES C)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

<<<<<<< HEAD
add_executable(${PROJECT_NAME}
    src/meta.c
    bstring/bstring/bstrlib.c
=======
add_library(${PROJECT_NAME}
    source/hkArray.c
    include/hkArray.h
    include/hkArrayT.h
    include/hkArrayT-primitives.h
>>>>>>> cd37d2c623b8148756fd62f4cb238023b57ae88d
)
include_directories(src/)
include_directories(bstring/bstring)
# set(CMAKE_FOLDER include)

# add_library(${PROJECT_NAME}
#     source/hkArray.c
#     include/hkArray.h
# )
# target_include_directories(${PROJECT_NAME} PUBLIC include/)
# target_link_libraries(haikal PUBLIC )

