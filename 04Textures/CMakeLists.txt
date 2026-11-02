cmake_minimum_required(VERSION 3.28.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILE_FEATURES OFF)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

add_executable(textures textures.cpp shader.h shader.cpp)

target_include_directories(textures PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../stb_image/
)

target_link_libraries(textures PRIVATE
    OpenGL::GL glfw glad_core
)


# 使用生成器表达式 $<TARGET_FILE_DIR:MyApp> 精确定位可执行文件所在目录
add_custom_command(TARGET textures POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/Shader.vs"
        "${CMAKE_CURRENT_SOURCE_DIR}/Shader.fs"
        "${CMAKE_CURRENT_SOURCE_DIR}/container.jpg"
        "${CMAKE_CURRENT_SOURCE_DIR}/awesomeface.png"
        "$<TARGET_FILE_DIR:textures>"
    COMMENT "Copying shaders to $<TARGET_FILE_DIR:textures>"
)



