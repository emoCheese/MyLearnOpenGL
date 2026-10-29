#include <cstddef>
#include <glad/glad.h>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);


struct Image 
{
	int width = 0;
	int height = 0;
	int nrChannels = 0;
	unsigned char* data = nullptr;
	// std::byte* data2;
};

Image readImage(const char* path);

/// ============ 顶点着色器 ==============
const char *vertexShaderSource = 
	"#version 460 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   	gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

/// ============ 片段着色器 ==============
const char* fragmentShaderSource = 
	"#version 460 core\n"
	"out vec4 FragColor;\n"
	"void main() {\n"
	"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\0";

int main(int, char**)
{
    	std::cout << "Hello, from hello_triangle!\n";


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
	constexpr float vertices[] = {
		//     ---- 位置 ----       		---- 颜色 ----     		- 纹理坐标 -
		0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
		0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
		-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
		-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};

	constexpr unsigned int indices[] = {  
		0, 1, 3, // first triangle
		1, 2, 3  // second triangle
	};

	
	/// 生成一个带有 ID 的 顶点缓冲对象 VBO，
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	/// 先绑定 VAO, 再绑定设置 VBO, and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	/// 把之前定义的顶点数据复制到缓冲的内存中
	/// 第四个参数指定了我们希望显卡如何管理给定的数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices),vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	static constexpr auto float_size = sizeof(float);
	/// float 32 位，4 byte, 
	/// 注意调整 步长为8
	glVertexAttribPointer(
		0, 3, 
		GL_FLOAT, 
		GL_FALSE,
		8 * float_size, (void*)0
	);

	/// glVertexAttribPointer之前绑定的是先前定义的VBO对象，顶点属性0现在会链接到它的顶点数据
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * float_size, (void*)(3 * float_size));
	glEnableVertexAttribArray(1);

	/// 传给 顶点着色器的 vec2 aTexCoord，然后再传给 片段着色器
	glVertexAttribPointer(
		2, 2,
		GL_FLOAT,
		GL_FALSE, 8 * float_size,
		(void*)(6 * float_size)
	);

	glEnableVertexAttribArray(2);


	/// 纹理加载生成
	/// =================================================
	auto image = readImage("container.jpg");
	if (image.data == nullptr) 
	{
		std::cerr << "Failed to load image container\n";
	}

	auto image_face = readImage("awesomeface.png");
	if (image.data == nullptr) 
	{
		std::cerr << "Failed to load face image\n";
	}

	// 生成纹理 纹理也是使用ID引用的
	unsigned int texture1, texture2;
	glGenTextures(1, &texture1);
	glGenTextures(1, &texture2);

	glBindTexture(GL_TEXTURE_2D, texture1);
	glBindTexture(GL_TEXTURE_2D, texture2);

	// 为当前绑定的纹理对象设置环绕、过滤方式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);   
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/**
	 * 第一个参数指定了纹理目标(Target)。设置为GL_TEXTURE_2D意味着会生成与当前绑定的纹理对象在同一个目标上的纹理（任何绑定到GL_TEXTURE_1D和GL_TEXTURE_3D的纹理不会受到影响）。
	 * 第二个参数为纹理指定多级渐远纹理的级别，如果你希望单独手动设置每个多级渐远纹理的级别的话。这里我们填0，也就是基本级别。
	 * 第三个参数告诉OpenGL我们希望把纹理储存为何种格式。我们的图像只有RGB值，因此我们也把纹理储存为RGB值。
	 * 第四个和第五个参数设置最终的纹理的宽度和高度。我们之前加载图像的时候储存了它们，所以我们使用对应的变量。
	 * 下个参数应该总是被设为0（历史遗留的问题）。
	 * 第七第八个参数定义了源图的格式和数据类型。我们使用RGB值加载这个图像，并把它们储存为char(byte)数组，我们将会传入对应值。
	 * 最后一个参数是真正的图像数据。
	 */
	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB,
		image.width, image.height, 0,
		GL_RGB, GL_UNSIGNED_BYTE, image.data
	);

	/// 为当前绑定的纹理自动生成所有需要的多级渐远纹理
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(image.data);



	glTexImage2D(
		GL_TEXTURE_2D, 0, GL_RGB, 
		image_face.width, image_face.height, 
		0, GL_RGBA, GL_UNSIGNED_BYTE, image_face.data
	);
	glGenerateMipmap(GL_TEXTURE_2D);


	// 请注意，这种操作是允许的。glVertexAttribPointer的调用会将VBO注册为该顶点属性所对应的顶点缓冲区对象。这样一来，我们之后就可以安全地解除该VBO的绑定。
    	// glBindBuffer(GL_ARRAY_BUFFER, 0); 

    	// 之后可以解除VAO的绑定，这样其他VAO的调用就不会意外地修改这个VAO了。不过这种情况很少发生。
	// 无论如何，要修改其他VAO的话，都需要先调用glBindVertexArray。因此，除非确实有必要，否则我们通常不会解除VAO或VBO的绑定。
    	// glBindVertexArray(0); 
	
	Shader shader("Shader.vs", "Shader.fs");
	

	glBindVertexArray(VAO);

	while (!glfwWindowShouldClose(window))
	{
		processInput(window);

		// render
		// ------
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();

		/// 会自动把纹理赋值给片段着色器的采样器 uniform sampler2D ourTexture，不清楚原理
		glBindTexture(GL_TEXTURE_2D, texture1);
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);


		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// optional: de-allocate all resources once they've outlived their purpose:
	// ------------------------------------------------------------------------
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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

Image readImage(const char* path) 
{
	Image ret;
	ret.data = stbi_load(path, &ret.width, &ret.height, &ret.nrChannels, 0);
	return ret;
}	