cmake_minimum_required(VERSION 3.28.0)
project(hello_window VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILE_FEATURES OFF)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(hello_window main.cpp)

target_link_libraries(hello_window PRIVATE
    OpenGL::GL OpenGL::GLU glfw glad_core
)   


