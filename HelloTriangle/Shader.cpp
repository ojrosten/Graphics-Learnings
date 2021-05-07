#include "Shader.h"

namespace Graphics
{
    void report(GLuint shader, std::string_view shaderName, bool success)
    {
        if(!success)
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

    int linkProgram(GLuint vertexShader, GLuint fragmentShader)
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

    ShaderCompiler::ShaderCompiler(GLuint shaderType, std::string_view shaderSource, std::string_view shaderName)
        : m_Shader{glCreateShader(shaderType)}
    {
        const auto pData{shaderSource.data()};
        glShaderSource(m_Shader, 1, &pData, NULL);
        glCompileShader(m_Shader);

        check(m_Shader, shaderName, [shader{m_Shader}](int* success) { glGetShaderiv(shader, GL_COMPILE_STATUS, success); });

    }

    ShaderProgram::ShaderProgram(const char* vertexPath, const char* fragmentPath)
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
        catch(std::ifstream::failure&)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* fShaderCode = fragmentCode.c_str();
        // 2. compile shaders

        const ShaderCompiler vertex{GL_VERTEX_SHADER, vShaderCode, "VERTEX"};
        const ShaderCompiler fragment{GL_FRAGMENT_SHADER, fShaderCode, "FRAGMENT"};


        // shader Program
        m_ID = linkProgram(vertex.get(), fragment.get());
    }

    void setUpUberPhong(ShaderProgram& program)
    {
        program.use();
        program.setVec3("light[0].ambient", 0.2f, 0.2f, 0.2f);
        program.setVec3("light[0].diffuse", 0.5f, 0.5f, 0.5f);
        program.setVec3("light[0].specular", 1.0f, 1.0f, 1.0f);
        program.setFloat("light[0].outerCutOff", glm::cos(glm::radians(17.0f)));
        program.setFloat("light[0].innerCutOff", glm::cos(glm::radians(12.0f)));
        program.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
        program.setInt("material.diffuse", 0);
        program.setInt("material.specular", 1);
        program.setFloat("material.shininess", 128);
    }

    void updateUberPhong(ShaderProgram& program,
        const glm::highp_mat4& view,
        const glm::highp_mat4& projection,
        const glm::mat4& model,
        const Lighting& light,
        const camera& c)
    {
        program.use();
        program.setMatrix4("view", view);
        program.setMatrix4("projection", projection);
        program.setMatrix4("model", model);
        program.setVec4("light[0].directionality", light.directionality());
        program.setFloat("light[0].constant", 1.0f);
        program.setFloat("light[0].linear", 0.09f);
        program.setFloat("light[0].quadratic", 0.032f);
        program.setVec3("light[0].ambient", 0.2f, 0.2f, 0.2f);
        program.setVec3("light[0].diffuse", 0.5f, 0.5f, 0.5f);
        program.setVec3("light[0].specular", 1.0f, 1.0f, 1.0f);
        program.setFloat("light[0].outerCutOff", glm::cos(glm::radians(15.0f)));
        program.setFloat("light[0].innerCutOff", glm::cos(glm::radians(12.0f)));
        program.setVec3("viewPos", c.pos);
    }
}