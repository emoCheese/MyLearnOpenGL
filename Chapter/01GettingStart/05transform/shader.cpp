#include "shader.h"
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <system_error>

#include <filesystem>
#include <string>
#include <vector>
#include <cstddef>

bool isFilePath(const char* path) {
    if (path == nullptr || path[0] == '\0') return false; // 空指针或空串

    std::error_code ec;
    auto p = std::filesystem::path(path);

    // 必须存在，且是普通文件（不是目录）
    return std::filesystem::exists(p, ec) && !ec
        && std::filesystem::is_regular_file(p, ec) && !ec;
}

void print_path(const char* path) {
	std::error_code ec;
	std::cout << "resolved path: "
		<< std::filesystem::absolute(path, ec)
		<< "\n";
	std::cout << "exists: " << std::filesystem::exists(path)
		<< ", last_write: ";
	// 打印修改时间，方便对比
	auto ftime = std::filesystem::last_write_time(path, ec);
	std::cout << ftime.time_since_epoch().count() << "\n";
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;

	const char* vShaderCode = nullptr;
    	const char* fShaderCode = nullptr;

	bool is_file = isFilePath(vertexPath) && isFilePath(fragmentPath);
	if (is_file)
	{
		std::printf("从文件读取\n");
		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		// 保证ifstream对象可以抛出异常：
		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

		try 
		{
			// 打开文件
			vShaderFile.open(vertexPath);
			print_path(vertexPath);
			fShaderFile.open(fragmentPath);
			print_path(fragmentPath);
			
			std::stringstream vShaderStream, fShaderStream;
			// 读取文件的缓冲内容到数据流中
			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		} 
		catch (std::ifstream::failure e) 
		{
			std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		vShaderCode = vertexCode.c_str();
		fShaderCode = fragmentCode.c_str();

	}
	else 
	{
		std::printf("从字符串读取\n");
		vShaderCode = vertexPath;
		fShaderCode = fragmentPath;
	}
	

	// 2. 编译着色器
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// 创建着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	// 着色器源码附加到着色器对象上，编译它
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	// 打印编译错误（如果有的话）
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	

	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	// 打印编译错误（如果有的话）
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success)
	{
		std::memset(infoLog, '\0', sizeof(infoLog));
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	};

	// 着色器程序
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// 打印连接错误（如果有的话）
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
	}

	// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
	glDeleteShader(vertex);
	glDeleteShader(fragment);


	GLint numUniforms = 0;
	glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &numUniforms);
	std::cout << "active uniforms = " << numUniforms << "\n";
	for (GLint i = 0; i < numUniforms; ++i) {
		char name[256] = {};
		GLsizei len = 0;
		GLint size = 0;
		GLenum type = 0;
		glGetActiveUniform(ID, i, 255, &len, &size, &type, name);
		std::cout << "  [" << i << "] " << name << "\n";
	}
}

void Shader::use()
{
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

