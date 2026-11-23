#include "shader.h"
#include "glm/gtc/type_ptr.hpp"
#include <cstring>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <filesystem>
#include <system_error>

namespace {

// 编辑器有时会用 “UTF-8 with BOM” 保存着色器。BOM 不属于 GLSL 空白字符，
// 会让 #version 不再是第一个有效记号，导致编译直接失败。这里统一剥离。
void stripUtf8Bom(std::string& code)
{
	if (code.size() >= 3 &&
		static_cast<unsigned char>(code[0]) == 0xEF &&
		static_cast<unsigned char>(code[1]) == 0xBB &&
		static_cast<unsigned char>(code[2]) == 0xBF)
	{
		code.erase(0, 3);
	}
}

} // namespace

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

			// 兼容带 UTF-8 BOM 的着色器文件
			stripUtf8Bom(vertexCode);
			stripUtf8Bom(fragmentCode);
		} 
		catch (std::ifstream::failure e) 
		{
			std::cerr << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}

		if (vertexCode.empty() || fragmentCode.empty())
		{
			std::cerr << "ERROR::SHADER::FILE_IS_EMPTY" << std::endl;
			return;
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

	// 创建着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	// 着色器源码附加到着色器对象上，编译它
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	const bool vertexOk = checkCompileErrors(vertex, "VERTEX");

	glShaderSource(fragment, 1, &fShaderCode, nullptr);
	glCompileShader(fragment);
	const bool fragmentOk = checkCompileErrors(fragment, "FRAGMENT");

	// 任一个阶段编译失败都不要继续链接：把未编译的着色器附加到程序上，
	// 只会再产生一次链接错误，并让 ID 变成一个不可用的程序对象。
	if (!vertexOk || !fragmentOk)
	{
		glDeleteShader(vertex);
		glDeleteShader(fragment);
		return;
	}

	// 着色器程序
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	if (!checkCompileErrors(ID, "PROGRAM"))
	{
		glDeleteProgram(ID);
		ID = 0;
	}

	// 删除着色器，它们已经链接到我们的程序中了，已经不再需要了
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	if (ID == 0)
	{
		return;
	}

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
	if (ID == 0) return;
	glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
	if (ID == 0) return;
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
	if (ID == 0) return;
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
	if (ID == 0) return;
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const glm::mat4& value) const
{
	if (ID == 0) return;
	const auto location = glGetUniformLocation(ID, name.c_str());
	glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::setMat3(const std::string &name, const glm::mat3 &mat) const 
{
	if (ID == 0) return;
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
	                   &mat[0][0]);
}

void Shader::setMat2(const std::string &name, const glm::mat2 &mat) const 
{
	if (ID == 0) return;
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE,
	                   &mat[0][0]);
}

void Shader::setVec4(const std::string &name, float x, float y, float z, float w) const 
{
	if (ID == 0) return;
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

void Shader::setVec4(const std::string &name, const glm::vec4 &value) const 
{
	if (ID == 0) return;
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec3(const std::string &name, float x, float y, float z) const 
{
	if (ID == 0) return;
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

void Shader::setVec3(const std::string &name, const glm::vec3 &value) const 
{
	if (ID == 0) return;
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

void Shader::setVec2(const std::string &name, float x, float y) const 
{
	if (ID == 0) return;
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

void Shader::setVec2(const std::string &name, const glm::vec2 &value) const 
{
	if (ID == 0) return;
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}
bool Shader::checkCompileErrors(GLuint shader, const std::string& type) const
{
	GLint success = 0;
	char infoLog[1024] = {};

	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			std::cerr
				<< "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
				<< infoLog
				<< "\n -- --------------------------------------------------- -- "
				<< std::endl;
			return false;
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			std::cerr
				<< "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
				<< infoLog
				<< "\n -- --------------------------------------------------- -- "
				<< std::endl;
			return false;
		}
	}

	return true;
}
