#include "Cubes.h"

#include "../Binder.h"
#include "../Shader.h"

namespace Graphics::Examples
{
  Cube::Cube(const std::filesystem::path& texPath, const std::filesystem::path& specMapPath)
    : texture{GL_TEXTURE0, texPath}
    , specTexture{GL_TEXTURE1, specMapPath}
  {
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices.data(), GL_STATIC_DRAW); // VBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, cols * sizeof(float), (void*)0); // VAO
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, cols * sizeof(float), (void*)(3 * sizeof(float))); // VAO
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, cols * sizeof(float), (void*)(5 * sizeof(float))); // VAO
    glEnableVertexAttribArray(2);
  }

  void Cube::Draw(Graphics::ShaderProgram&)
  {
    vbo.bind();
    vao.bind();
    ebo.bind();
    texture.bind();
    specTexture.bind();

    glDrawArrays(GL_TRIANGLES, 0, 36);
  }
}