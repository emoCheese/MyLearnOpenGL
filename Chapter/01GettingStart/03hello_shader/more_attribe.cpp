#include "shader.h"
#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


int main(int, char**)
{
    std::cout << "Hello, from hello_shader!\n";


	/// 初始化窗口
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	
#ifdef __APPLE__
    	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	GLFWwindow* window = glfwCreateWindow(800, 600, "Learn OpenGL", nullptr, nullptr);
	if (!window) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);     // 设为当前线程的主上下文
	// 我们还需要注册这个函数，告诉GLFW我们希望每当窗口调整大小的时候调用这个函数：
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


	// GLAD是用来管理OpenGL的函数指针的，所以在调用任何OpenGL的函数之前我们需要初始化GLAD。
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}


	// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
	/// VAO 顶点数组对象
	/// 绘制三角形，指定三个顶点
    	float vertices[] = {
		// 位置              		// 颜色
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
	};
	
	/// 生成一个带有 ID 的 顶点缓冲对象 VBO，
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	/// 先绑定 VAO, 再绑定设置 VBO, and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	/// 把之前定义的顶点数据复制到缓冲的内存中
	/// 第四个参数指定了我们希望显卡如何管理给定的数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);


	/// 设置位置属性
	glVertexAttribPointer(
		0, 3, 
		GL_FLOAT, 
		GL_FALSE,
		6 * sizeof(float), (void*)0
	);

	/// glVertexAttribPointer之前绑定的是先前定义的VBO对象，顶点属性0现在会链接到它的顶点数据
	glEnableVertexAttribArray(0);

	/// 设置颜色属性
	glVertexAttribPointer(
		1, 3, 
		GL_FLOAT, 
		GL_FALSE,
		6 * sizeof(float), (void*)(3 * sizeof(float))
	);

	glEnableVertexAttribArray(1);

	// 请注意，这种操作是允许的。glVertexAttribPointer的调用会将VBO注册为该顶点属性所对应的顶点缓冲区对象。这样一来，我们之后就可以安全地解除该VBO的绑定。
    	glBindBuffer(GL_ARRAY_BUFFER, 0); 

    	// 之后可以解除VAO的绑定，这样其他VAO的调用就不会意外地修改这个VAO了。不过这种情况很少发生。
	// 无论如何，要修改其他VAO的话，都需要先调用glBindVertexArray。因此，除非确实有必要，否则我们通常不会解除VAO或VBO的绑定。
    	glBindVertexArray(0); 


	Shader shader("Shader.vs", "Shader.fs");

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();
		glBindVertexArray(VAO);

		glDrawArrays(GL_TRIANGLES, 0, 3);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	// glDeleteProgram(shaderProgram);

	glfwTerminate();
	return 0;
}



void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}