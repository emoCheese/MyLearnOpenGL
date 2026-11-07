add_executable(transform transform.cpp shader.h shader.cpp)

target_link_libraries(transform PRIVATE
    OpenGL::GL glfw glad_core glm::glm stb_image
)


# 使用生成器表达式 $<TARGET_FILE_DIR:MyApp> 精确定位可执行文件所在目录
add_custom_command(TARGET transform POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/Shader.vs"
        "${CMAKE_CURRENT_SOURCE_DIR}/Shader.fs"
        "${CMAKE_CURRENT_SOURCE_DIR}/container.jpg"
        "${CMAKE_CURRENT_SOURCE_DIR}/awesomeface.png"
        "$<TARGET_FILE_DIR:transform>"
    COMMENT "Copying shaders to $<TARGET_FILE_DIR:transform>"
)



