#pragma once

#include "Shader.h"
#include "Binder.h"

#include "glm/glm.hpp"

#include<string>
#include<vector>

namespace Graphics
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
    };

    struct Texture
    {
        unsigned int id;
        std::string type;
        std::filesystem::path file;
    };

    class Mesh {
    public:
        // mesh data
        Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
        void Draw(Graphics::ShaderProgram& shader) const;
    private:

        std::vector<Vertex>       m_Vertices;
        std::vector<unsigned int> m_Indices;
        std::vector<Texture>      m_Textures;

        VAOBinder m_VAO;
        VBOBinder m_VBO;
        EBOBinder m_EBO;

        //unsigned int m_VAO, m_VBO, m_EBO;

        void setupMesh();
    };
}