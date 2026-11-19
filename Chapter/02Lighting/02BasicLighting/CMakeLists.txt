add_executable(BasicLighting "BasicLighting.cpp" "camera.h")


target_link_libraries(BasicLighting PRIVATE
        OpenGL::GL glfw glad_core glm::glm my_shader stb_image my_gl_utility my_glwindow
)

# 使用生成器表达式 $<TARGET_FILE_DIR:MyApp> 精确定位可执行文件所在目录
add_custom_command(TARGET BasicLighting POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/basic_light.vs.glsl"
        "${CMAKE_CURRENT_SOURCE_DIR}/basic_light.fs.glsl"
        "${CMAKE_CURRENT_SOURCE_DIR}/1light_cube.vs"
        "${CMAKE_CURRENT_SOURCE_DIR}/1light_cube.fs"
        "$<TARGET_FILE_DIR:BasicLighting>"
    COMMENT "Copying shaders to $<TARGET_FILE_DIR:BasicLighting>"
)
