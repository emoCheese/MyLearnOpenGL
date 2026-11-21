add_library(camera INTERFACE camera.h)

target_include_directories(camera INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(camera
        INTERFACE
                glad_core OpenGL::GL glm::glm 
)