add_executable(light_casters_spot_soft light_casters_spot_soft.cpp)

target_compile_features(light_casters_spot_soft PRIVATE cxx_std_20)

target_link_libraries(light_casters_spot_soft PRIVATE
        OpenGL::GL glfw glad_core glm::glm my_shader stb_image my_gl_utility camera my_glwindow
)

# 使用生成器表达式 $<TARGET_FILE_DIR:MyApp> 精确定位可执行文件所在目录
add_custom_command(TARGET light_casters_spot_soft POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        "${CMAKE_CURRENT_SOURCE_DIR}/5.4.light_casters.fs.glsl"
        "${CMAKE_CURRENT_SOURCE_DIR}/5.4.light_casters.vs.glsl" 
        "${CMAKE_CURRENT_SOURCE_DIR}/5.1.light_cube.fs.glsl"
        "${CMAKE_CURRENT_SOURCE_DIR}/5.1.light_cube.vs.glsl"
        "${CMAKE_CURRENT_SOURCE_DIR}/../container2.png"
        "${CMAKE_CURRENT_SOURCE_DIR}/../container2_specular.png"
        "$<TARGET_FILE_DIR:light_casters_spot_soft>"
    COMMENT "Copying shaders to $<TARGET_FILE_DIR:light_casters_spot_soft>"
)
