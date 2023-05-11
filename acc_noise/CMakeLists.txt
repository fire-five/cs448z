cmake_minimum_required(VERSION 3.22.1)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
SET (CMAKE_C_COMPILER "/usr/bin/gcc")
SET (CMAKE_CXX_COMPILER "/usr/bin/g++")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIE")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIE")
project(acc_noise)
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/src
)
find_package(glfw3 REQUIRED)
file(GLOB project_file
    "${PROJECT_SOURCE_DIR}/include/*.h"
    "${PROJECT_SOURCE_DIR}/src/*.c"
    "${PROJECT_SOURCE_DIR}/src/*.cpp"
)
add_executable(${PROJECT_NAME} ${project_file})

target_link_libraries(${PROJECT_NAME}
    -lglfw
    -lGL
    -lopenal
    -lsndfile
    -lm
    -lXrandr
    -lXi
    -lX11
    -lXxf86vm
    -lpthread
    -ldl
    -lXinerama
    -lXcursor
    )