add_executable(camera_class camera_class.cpp shader_m.h camera.h)


target_link_libraries(camera_class PRIVATE
        OpenGL::GL glfw glad_core glm::glm my_shader stb_image my_gl_utility
)

# 使用生成器表达式 $<TARGET_FILE_DIR:MyApp> 精确定位可执行文件所在目录
add_custom_command(TARGET camera_class POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/Shader.vs"
        "${CMAKE_CURRENT_SOURCE_DIR}/Shader.fs"
        "${CMAKE_CURRENT_SOURCE_DIR}/container.jpg"
        "${CMAKE_CURRENT_SOURCE_DIR}/awesomeface.png"
        "$<TARGET_FILE_DIR:camera_class>"
    COMMENT "Copying shaders to $<TARGET_FILE_DIR:camera_class>"
)
