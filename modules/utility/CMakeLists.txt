
add_library(my_gl_utility 
        STATIC 
                utility.h utility.cpp
)

target_include_directories(my_gl_utility 
        PUBLIC 
                ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(my_gl_utility
        PUBLIC
                OpenGL::GL 
                glfw 
                glad_core 
                glm::glm
                stb_image
)