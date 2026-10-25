#include <iostream>
#include <glad/glad.h>
#include <GL/gl.h>

int main(int, char**)
{
    	std::cout << "Hello, from hello_triangle!\n";

	/// VAO 顶点数组对象
	/// 绘制三角形，指定三个顶点
    	const float vertices[] = {
		//   x          y         z
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
    	};

	
	/// 生成一个带有 ID 的 顶点缓冲对象 VBO，
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	/// 把之前定义的顶点数据复制到缓冲的内存中
	/// 第四个参数指定了我们希望显卡如何管理给定的数据
	glBufferData(
		GL_ARRAY_BUFFER, 
		sizeof(vertices),
		vertices, 
		GL_STATIC_DRAW
	);

	/// ============ 顶点着色器 ==============
	const char *vertexShaderSource = "#version 460 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
	"}\0";

	// 创建着色器
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);


	// 着色器源码附加到着色器对象上，编译它
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	if (!success) 
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	/// ============ 片段着色器 ==============
	const char* fragmentShaderSource = 
		"#version 460 core\n"
		"out vec4 FragColor;\n"
		"void main() {\n"
		"	FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
		"}\0";


	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
	glCompileShader(fragmentShader);

	
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) 
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}


	/// ====== 着色器程序 ========
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if(!success) 
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR\n" << infoLog << std::endl;
	}

	/// 激活这个着色器程序
	glUseProgram(shaderProgram);

	/// 把着色器对象链接到程序对象以后，记得删除着色器对象，我们不再需要它们了：
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);


	/// float 32 位，4 byte,
	glVertexAttribPointer(
		0, 3, 
		GL_FLOAT, GL_FALSE,
		 3 * sizeof(float), (void*)0
	);

	glEnableVertexAttribArray(0);

}
