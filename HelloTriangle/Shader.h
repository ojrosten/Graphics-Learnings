#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include <filesystem>

namespace Graphics
{
    template<class Fn>
    void check(GLuint shader, std::string_view shaderName, Fn fn)
    {
        int success;
        fn(&success);

        if (!success)
        {
            char infoLog[512];
            glGetShaderInfoLog(shader, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << shaderName << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        else
        {
            std::cout << "Compiled " << shaderName << " successfully!\n";
        }
    }

    class ShaderCompiler
    {
        GLuint m_Shader;
    public:
        ShaderCompiler(GLuint shaderType, std::string_view shaderSource, std::string_view shaderName)
            : m_Shader{ glCreateShader(shaderType) }
        {
            const auto pData{ shaderSource.data() };
            glShaderSource(m_Shader, 1, &pData, NULL);
            glCompileShader(m_Shader);

            check(m_Shader, shaderName, [shader{ m_Shader }](int* success) { glGetShaderiv(shader, GL_COMPILE_STATUS, success); });

        }

        [[nodiscard]]
        GLuint get() const noexcept
        {
            return m_Shader;
        }

        ~ShaderCompiler()
        {
            glDeleteShader(m_Shader);
        }
    };

    inline int linkProgram(GLuint vertexShader, GLuint fragmentShader)
    {
        unsigned int shaderProgram;
        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);

        check(shaderProgram, "SHADER_PROGRAM",
            [shaderProgram](int* success) { glGetProgramiv(shaderProgram, GL_LINK_STATUS, success); });

        return shaderProgram;
    }

    class ShaderProgram
    {
    public:
        ShaderProgram(const char* vertexPath, const char* fragmentPath)
        {
            std::string vertexCode;
            std::string fragmentCode;
            std::ifstream vShaderFile;
            std::ifstream fShaderFile;
            // ensure ifstream objects can throw exceptions:
            vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
            try
            {
                // open files
                vShaderFile.open(vertexPath);
                fShaderFile.open(fragmentPath);
                std::stringstream vShaderStream, fShaderStream;
                // read file's buffer contents into streams
                vShaderStream << vShaderFile.rdbuf();
                fShaderStream << fShaderFile.rdbuf();
                // close file handlers
                vShaderFile.close();
                fShaderFile.close();
                // convert stream into string
                vertexCode = vShaderStream.str();
                fragmentCode = fShaderStream.str();
            }
            catch (std::ifstream::failure&)
            {
                std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            }
            const char* vShaderCode = vertexCode.c_str();
            const char* fShaderCode = fragmentCode.c_str();
            // 2. compile shaders

            const ShaderCompiler vertex{ GL_VERTEX_SHADER, vShaderCode, "VERTEX" };
            const ShaderCompiler fragment{ GL_FRAGMENT_SHADER, fShaderCode, "FRAGMENT" };


            // shader Program
            m_ID = linkProgram(vertex.get(), fragment.get());
        }

        void use()
        {
            glUseProgram(m_ID);
        }

        void setBool(std::string_view uniform, bool value) const
        {
            glUniform1i(glGetUniformLocation(m_ID, uniform.data()), (int)value);
        }
        // ------------------------------------------------------------------------
        void setInt(std::string_view uniform, int value) const
        {
            glUniform1i(glGetUniformLocation(m_ID, uniform.data()), value);
        }
        // ------------------------------------------------------------------------
        void setFloat(std::string_view uniform, float value) const
        {
            glUniform1f(glGetUniformLocation(m_ID, uniform.data()), value);
        }

        void setVec3(std::string_view uniform, const glm::vec3& v)
        {
            setVec3(uniform, v.x, v.y, v.z);
        }

        void setVec3(std::string_view uniform, float x, float y, float z)
        {
            const int loc{ glGetUniformLocation(m_ID, uniform.data()) };
            glUniform3f(loc, x, y, z);
        }

        void setVec4(std::string_view uniform, float x, float y, float z, float w)
        {
            const int loc{ glGetUniformLocation(m_ID, uniform.data()) };
            glUniform4f(loc, x, y, z, w);
        }

        void setVec4(std::string_view uniform, const glm::vec4& v)
        {
            const int loc{glGetUniformLocation(m_ID, uniform.data())};
            glUniform4f(loc, v.x, v.y, v.z, v.w);
        }

        void setMatrix4(std::string_view uniform, const glm::mat4& mat)
        {
            const int loc{ glGetUniformLocation(m_ID, uniform.data()) };
            glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
        }

        void setTexture(std::string_view name, int index)
        {
            glUniform1i(glGetUniformLocation(m_ID, name.data()), index);
        }
    private:
        int  m_ID{};

        std::string readCode()
        {

        }
    };
}
