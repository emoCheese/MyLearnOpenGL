cmake_minimum_required(VERSION 3.28.0)
project(LearnOpenGL VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILE_FEATURES OFF)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)


add_subdirectory(glad_core)
add_subdirectory(glm)
add_subdirectory(hello_window)
add_subdirectory(hello_triangle)
add_subdirectory(hello_shader)
add_subdirectory(Textures)
