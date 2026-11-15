add_library(my_glwindow STATIC my_glwindow.h my_glwindow.cpp)


target_include_directories(my_glwindow PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(my_glwindow
        PUBLIC
                OpenGL::GL 
                glfw
                glad_core
)