#pragma once
#include <glm/ext/matrix_float4x4.hpp>
#include <glad/glad.h>
#include <string>


class Shader
{
public:
    // 程序ID，构造失败时保持为 0
    unsigned int ID = 0;

    // 构造器读取并构建着色器
    Shader(const char* vertexPath, const char* fragmentPath);
    // 使用/激活程序
    void use();
    // uniform工具函数
    void setBool(const std::string& name, bool value) const;  
    void setInt(const std::string& name, int value) const;   
    void setFloat(const std::string& name, float value) const;

        // ------------------------------------------------------------------------
    void setVec2(const std::string &name, const glm::vec2 &value) const;
    void setVec2(const std::string &name, float x, float y) const;
    // ------------------------------------------------------------------------
    void setVec3(const std::string &name, const glm::vec3 &value) const;
    void setVec3(const std::string &name, float x, float y, float z) const;
    // ------------------------------------------------------------------------
    void setVec4(const std::string &name, const glm::vec4 &value) const;
    void setVec4(const std::string &name, float x, float y, float z, float w) const;
    // ------------------------------------------------------------------------
    void setMat2(const std::string &name, const glm::mat2 &mat) const;
    // ------------------------------------------------------------------------
    void setMat3(const std::string &name, const glm::mat3 &mat) const;
    // ------------------------------------------------------------------------
    void setMat4(const std::string &name, const glm::mat4 &mat) const;

    // utility function for checking shader compilation/linking errors.
    // 返回 true 表示编译/链接成功，false 表示失败。
    // ------------------------------------------------------------------------
    bool checkCompileErrors(GLuint shader, const std::string& type) const;
};


