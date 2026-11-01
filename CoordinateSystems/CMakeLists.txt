add_executable(CoordinateSystems coordinate.cpp shader.h shader.cpp)

target_include_directories(CoordinateSystems PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../stb_image/
)


target_link_libraries(CoordinateSystems PRIVATE
        OpenGL::GL glfw glad_core glm::glm
)

# 使用生成器表达式 $<TARGET_FILE_DIR:MyApp> 精确定位可执行文件所在目录
add_custom_command(TARGET CoordinateSystems POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/Shader.vs"
        "${CMAKE_CURRENT_SOURCE_DIR}/Shader.fs"
        "${CMAKE_CURRENT_SOURCE_DIR}/container.jpg"
        "${CMAKE_CURRENT_SOURCE_DIR}/awesomeface.png"
        "$<TARGET_FILE_DIR:CoordinateSystems>"
    COMMENT "Copying shaders to $<TARGET_FILE_DIR:CoordinateSystems>"
)