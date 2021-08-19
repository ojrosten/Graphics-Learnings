#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Camera.h"
#include "Lighting.h"
#include "Materials.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <filesystem>

namespace Graphics
{
    void report(GLuint shader, std::string_view shaderName, bool success);

    template<class Fn>
    void check(GLuint shader, std::string_view shaderName, Fn fn)
    {
        int success;
        fn(&success);
        report(shader, shaderName, success);
    }

    class ShaderCompiler
    {
        GLuint m_Shader;
    public:
        ShaderCompiler(GLuint shaderType, std::string_view shaderSource, std::string_view shaderName);

        ShaderCompiler(ShaderCompiler&& other)
            : m_Shader{std::exchange(other.m_Shader, 0)}
        {}

        ShaderCompiler& operator=(ShaderCompiler&& other)
        {
            m_Shader = std::exchange(other.m_Shader, 0);
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

    int linkProgram(GLuint vertexShader, GLuint fragmentShader);

    class ShaderProgram
    {
    public:
        ShaderProgram(std::filesystem::path vertexShader, std::filesystem::path fragmentShader);

        ShaderProgram(ShaderProgram&& other) noexcept
            : m_ID{std::exchange(other.m_ID, 0)}
        {}

        ShaderProgram& operator=(ShaderProgram&& other) noexcept
        {
            m_ID = std::exchange(other.m_ID, 0);
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
    };

    void updateUberPhong(ShaderProgram& program,
                         const glm::highp_mat4& view,
                         const glm::highp_mat4& projection,
                         const glm::mat4& model,
                         const std::vector<Lighting>& lights,
                         const MaterialVariant& material,
                         const float alphaThreshold,
                         const camera& c);
}
