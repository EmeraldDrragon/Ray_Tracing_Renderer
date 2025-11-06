cmake_minimum_required(VERSION 3.10)
project(Ray_Tracer)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(ray_tracer
    source/main.cpp
    source/Renderer.cpp
    source/Surface.cpp)

target_include_directories(ray_tracer PRIVATE include)

file(GLOB OBJ_FILES "${CMAKE_CURRENT_SOURCE_DIR}/obj_files/*.obj")

file(COPY ${OBJ_FILES}
    DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})