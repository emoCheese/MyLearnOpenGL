add_executable(Material "Material.cpp" )


target_link_libraries(Material PRIVATE
        OpenGL::GL glfw glad_core glm::glm my_shader stb_image my_gl_utility my_glwindow camera
)

# 使用生成器表达式 $<TARGET_FILE_DIR:MyApp> 精确定位可执行文件所在目录
add_custom_command(TARGET Material POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/3.1.materials.fs.glsl"
        "${CMAKE_CURRENT_SOURCE_DIR}/3.1.materials.vs.glsl"
        "${CMAKE_CURRENT_SOURCE_DIR}/3.1.light_cube.vs.glsl"
        "${CMAKE_CURRENT_SOURCE_DIR}/3.1.light_cube.fs.glsl"
        "$<TARGET_FILE_DIR:Material>"
    COMMENT "Copying shaders to $<TARGET_FILE_DIR:Material>"
)
