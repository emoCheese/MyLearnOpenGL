

add_library(glad_core STATIC
    include/glad/glad.h
    include/KHR/khrplatform.h
    src/glad.c
)


target_include_directories(glad_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(glad_core PRIVATE
    OpenGL::GL glfw
)