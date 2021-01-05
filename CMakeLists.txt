cmake_minimum_required(VERSION 3.17)
project(lab3_comp_graphics)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(lab3_comp_graphics additional/glad.c main.cpp Shader.h Camera.h Engine.cpp Engine.h Sphere.cpp Sphere.h Cubemap.cpp Cubemap.h)

find_package(glfw3 REQUIRED)
target_link_libraries(lab3_comp_graphics GL glfw)
