cmake_minimum_required(VERSION 3.28.0)
project(hello_shader VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILE_FEATURES OFF)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(hello_shader  hello_shader.cpp)


target_link_libraries(hello_shader PRIVATE
    OpenGL::GL glfw  glad_core
)   

add_executable(more_attribe more_attribe.cpp shader.h shader.cpp)

target_link_libraries(more_attribe PRIVATE
    OpenGL::GL glfw  glad_core
)   

# 使用生成器表达式 $<TARGET_FILE_DIR:MyApp> 精确定位可执行文件所在目录
add_custom_command(TARGET more_attribe POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/Shader.vs"
        "${CMAKE_CURRENT_SOURCE_DIR}/Shader.fs"
        "$<TARGET_FILE_DIR:more_attribe>"
    COMMENT "Copying shaders to $<TARGET_FILE_DIR:more_attribe>"
)



add_executable(hello_shader_exercise1 hello_shader_exercise1.cpp shader.h shader.cpp)

target_link_libraries(hello_shader_exercise1 PRIVATE
    OpenGL::GL glfw  glad_core
)   

# 使用生成器表达式 $<TARGET_FILE_DIR:MyApp> 精确定位可执行文件所在目录
add_custom_command(TARGET hello_shader_exercise1 POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/Shader.vs"
        "${CMAKE_CURRENT_SOURCE_DIR}/Shader.fs"
        "$<TARGET_FILE_DIR:hello_shader_exercise1>"
    COMMENT "Copying shaders to $<TARGET_FILE_DIR:hello_shader_exercise1>"
)


