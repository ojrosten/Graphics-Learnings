#include "Shader.h"
#include "Utilities.h"

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

    ShaderProgram::ShaderProgram(std::filesystem::path vertexShader, std::filesystem::path fragmentShader)
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
            vShaderFile.open(vertexShader.string().data());
            fShaderFile.open(fragmentShader.string().data());
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

    void updateUberPhong(ShaderProgram& program,
                         const glm::highp_mat4& view,
                         const glm::highp_mat4& projection,
                         const glm::mat4& model,
                         const std::vector<Lighting>& lights,
                         const MaterialVariant& material,
                         const float alphaThreshold,
                         const camera& c)
    {
        program.use();
        program.setMatrix4("view", view);
        program.setMatrix4("projection", projection);
        program.setMatrix4("model", model);
        program.setVec3("viewPos", c.pos);
        program.setInt("numLights", static_cast<int>(lights.size()));
        program.setFloat("alphaThreshold", alphaThreshold);

        for(int i{}; i < lights.size(); ++i)
        {
            const auto& light{lights[i]};
            auto preamble{std::string{"light["}.append(std::to_string(i)).append("].")};

            program.setVec4(preamble + "directionality", light.directionality());
            program.setVec3(preamble + "ambient",  light.components().ambient);
            program.setVec3(preamble + "diffuse",  light.components().diffuse);
            program.setVec3(preamble + "specular", light.components().specular);
            program.setFloat(preamble + "innerCutOff", light.shape().cosInnerCutoff);
            program.setFloat(preamble + "outerCutOff", light.shape().cosOuterCutoff);
            program.setFloat(preamble + "constant", light.attenuation().constant);
            program.setFloat(preamble + "linear", light.attenuation().linear);
            program.setFloat(preamble + "quadratic", light.attenuation().quadratic);
        }

        auto visitor{
            Utilities::overloaded{
                [&program](const Material& m) {
                    program.setBool("material.textured", false);
                    program.setVec3("material.ambient", m.ambient);
                    program.setVec3("material.diffuse", m.diffuse);
                    program.setVec3("material.specular", m.specular);
                },
                [&program](const MappedMaterial& m) {
                    program.setBool("material.textured", true);
                    program.setInt("material.diffuseSampler", m.diffuseIndex);
                    program.setInt("material.specularSampler", m.specularIndex);
                    program.setFloat("material.shininess", m.shininess);
                }
            }
        };

        std::visit(visitor, material);
    }
}