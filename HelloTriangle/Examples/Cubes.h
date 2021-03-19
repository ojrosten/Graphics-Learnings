#pragma once

#include "../Binder.h"

#include <array>
#include <filesystem>

namespace Graphics
{
  class ShaderProgram;
}

namespace Graphics::Examples
{
  class Cube
  {
  public:
    Cube(const std::filesystem::path& texPath);

    void Draw(Graphics::ShaderProgram&);
  private:
    VBOBinder vbo{};
    VAOBinder vao{};
    EBOBinder ebo{};
    TextureBinder texture;

    constexpr static std::size_t cols{8};
    constexpr static std::size_t rows{36};

    constexpr static std::array<unsigned, 6> indices{
        0, 1, 3,   // first triangle
        1, 2, 3    // second triangle
    };

    constexpr static std::array<float, cols* rows> vertices{
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f, 0.0f, -1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 0.0f, -1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 0.0f, -1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f, 0.0f, -1.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 0.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f, 0.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 0.0f, 1.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f,  0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  -1.0f, 0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f,  0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  -1.0f, 0.0f,  0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  -1.0f, 0.0f,  0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  -1.0f, 0.0f,  0.0f,

       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f,  0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f, 0.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f, 0.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f, 0.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f, 0.0f,  0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f,  0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f, 1.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f,  0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f, 1.0f,  0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f, 1.0f,  0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f, 1.0f,  0.0f,
    };
  };
}