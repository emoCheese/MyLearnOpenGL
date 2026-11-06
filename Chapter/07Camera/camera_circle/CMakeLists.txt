add_executable(camera_circle camera_circle.cpp)
target_include_directories(camera_circle PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../../stb_image/
)
target_link_libraries(camera_circle PRIVATE
        OpenGL::GL glfw glad_core glm::glm my_shader stb_image
)

# 使用生成器表达式 $<TARGET_FILE_DIR:MyApp> 精确定位可执行文件所在目录
add_custom_command(TARGET camera_circle POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/Shader.vs"
        "${CMAKE_CURRENT_SOURCE_DIR}/Shader.fs"
        "${CMAKE_CURRENT_SOURCE_DIR}/container.jpg"
        "${CMAKE_CURRENT_SOURCE_DIR}/awesomeface.png"
        "$<TARGET_FILE_DIR:camera_circle>"
    COMMENT "Copying shaders to $<TARGET_FILE_DIR:camera_circle>"
)
