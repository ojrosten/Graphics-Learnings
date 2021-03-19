#include "Mesh.h"

namespace Graphics
{
    Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
        : m_Vertices{std::move(vertices)}
        , m_Indices{std::move(indices)}
        , m_Textures{std::move(textures)}
    {
        setupMesh();
    }

    void Mesh::Draw(Graphics::ShaderProgram& shader) const
    {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalNr{ 1 }, heightNr{ 1 };

        for (unsigned int i = 0; i < m_Textures.size(); i++)
        {
            glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
            // retrieve texture number (the N in diffuse_textureN)

            const std::string name = m_Textures[i].type;
            const auto number{
                [&name,&diffuseNr,&specularNr,&normalNr,&heightNr]() {
                    if (name == "texture_diffuse")
                        return std::to_string(diffuseNr++);
                    
                    if (name == "texture_specular")
                        return std::to_string(specularNr++);

                    if (name == "texture_normal")
                        return std::to_string(normalNr++);

                    if (name == "texture_height")
                        return std::to_string(heightNr++);

                    throw std::logic_error{"Unrecognized Option"};
                }()
            };

            shader.setFloat(std::string{/*"material."*/}.append(name).append(number), static_cast<float>(i));
            glBindTexture(GL_TEXTURE_2D, m_Textures[i].id);
        }
        glActiveTexture(GL_TEXTURE0);

        // draw mesh
        glBindVertexArray(m_VAO.get());
//        glBindVertexArray(m_VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_Indices.size()), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glActiveTexture(GL_TEXTURE0);
    }

    void Mesh::setupMesh()
    {
        if (m_Vertices.empty()) throw std::runtime_error{"Empty vertices"};
        if (m_Indices.empty()) throw std::runtime_error{ "Empty indices" };

        /*glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);
        glGenBuffers(1, &m_EBO);
        glBindVertexArray(m_VAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);*/

        glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(unsigned int),
            &m_Indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));
        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

        glBindVertexArray(0);
    }
}