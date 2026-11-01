add_library(my_shader STATIC shader.h shader.cpp)

target_include_directories(my_shader PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(my_shader
        PRIVATE
                glad_core OpenGL::GL glfw glm::glm
)