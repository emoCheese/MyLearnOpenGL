#include "my_glwindow.h"
#include <iostream>


glWindow::glWindow(const char* title, unsigned int width, unsigned int height)
        : m_title(title), m_width(width), m_height(height), m_window(nullptr)
{
         
}

glWindow::~glWindow()
{
        glfwTerminate();
        std::cout << "Window Terminate\n";
}


bool glWindow::initWindow(int major, int minor)
{
        glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, major);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, minor);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        m_window = glfwCreateWindow(m_width, m_height, m_title.c_str(), nullptr, nullptr);  
        if (!m_window) 
        {
                std::cerr << "Failed to Create GLFW window\n";
                glfwTerminate();
                return false;
        }

        /// 设置GLFW 上下文为 window
        glfwMakeContextCurrent(m_window);
        return true;
}

