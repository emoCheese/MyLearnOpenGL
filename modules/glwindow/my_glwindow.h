#pragma once
#include <glad/glad.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <GLFW/glfw3.h>
#include <utility>
#include <string>


	// /// 初始化窗口
	// // glfw: initialize and configure
	// // ------------------------------
	// glfwInit();
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	// glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	// glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

using WindowSizeCallback = void(*)(GLFWwindow* window, int width, int height);
using MouseCallback = void(*)(GLFWwindow* window, double xposIn, double yposIn);
using ScrollCallback = void(*)(GLFWwindow* window, double xoffset, double yoffset);

class glWindow 
{
public:
        glWindow(const char* title, unsigned int width, unsigned int height);
        ~glWindow();

        inline auto* window() const { return m_window; }
        const char* title() const { return m_title.c_str(); }


        bool initWindow(int major, int minor);

        inline void setWindowSizeCallback(WindowSizeCallback framebuffer_size_callback) const 
        { glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback); }

        inline void setMouseCallback(MouseCallback mouse_callback) const 
        { 
                glfwSetCursorPosCallback(m_window, mouse_callback); 
                /// 设置 GLFW 捕捉鼠标事件
                glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }

        inline void setScrollCallback(ScrollCallback scroll_callback) const 
        { glfwSetScrollCallback(m_window, scroll_callback); }

        template<typename Func>
        void start(Func&& fn) const
        {
                while(!glfwWindowShouldClose(m_window))
                {
                        std::forward<Func>(fn)();

                	glfwSwapBuffers(m_window);
		        glfwPollEvents();
                }
        }

private:
        std::string m_title;
        unsigned int m_width;
        unsigned int m_height;
        GLFWwindow* m_window;
};