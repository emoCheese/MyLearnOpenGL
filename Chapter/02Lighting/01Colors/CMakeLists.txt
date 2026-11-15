add_executable(colors colors.cpp camera.h)


target_link_libraries(colors PRIVATE
        OpenGL::GL glfw glad_core glm::glm my_shader stb_image my_gl_utility my_glwindow
)

# 使用生成器表达式 $<TARGET_FILE_DIR:MyApp> 精确定位可执行文件所在目录
add_custom_command(TARGET colors POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/1colors.vs"
        "${CMAKE_CURRENT_SOURCE_DIR}/1colors.fs"
        "${CMAKE_CURRENT_SOURCE_DIR}/1light_cube.vs"
        "${CMAKE_CURRENT_SOURCE_DIR}/1light_cube.fs"
        "$<TARGET_FILE_DIR:colors>"
    COMMENT "Copying shaders to $<TARGET_FILE_DIR:colors>"
)
