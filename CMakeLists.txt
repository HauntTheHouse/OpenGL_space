cmake_minimum_required(VERSION 3.17)
project(OpenGL_space)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

add_executable(OpenGL_space main.cpp Shader.h Camera.h Engine.cpp Engine.h Sphere.cpp Sphere.h Cubemap.cpp Cubemap.h)

find_package(glfw3 REQUIRED)
target_link_libraries(OpenGL_space GL glfw GLEW)
