add_executable(lighting_maps_specular "lighting_maps_specular.cpp" )


target_link_libraries(lighting_maps_specular PRIVATE
        OpenGL::GL glfw glad_core glm::glm my_shader stb_image my_gl_utility my_glwindow camera
)

# 使用生成器表达式 $<TARGET_FILE_DIR:MyApp> 精确定位可执行文件所在目录
add_custom_command(TARGET lighting_maps_specular POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/4.2.lighting_maps.fs.glsl"
        "${CMAKE_CURRENT_SOURCE_DIR}/4.2.lighting_maps.vs.glsl"
        "${CMAKE_CURRENT_SOURCE_DIR}/4.1.light_cube.vs.glsl"
        "${CMAKE_CURRENT_SOURCE_DIR}/4.1.light_cube.fs.glsl"
        "${CMAKE_CURRENT_SOURCE_DIR}/../container2.png"
        "${CMAKE_CURRENT_SOURCE_DIR}/../container2_specular.png"
        "$<TARGET_FILE_DIR:lighting_maps_specular>"
    COMMENT "Copying shaders to $<TARGET_FILE_DIR:lighting_maps_specular>"
)
