#pragma once

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
    class Quad
    {
    public:
        Quad(const std::filesystem::path& texPath, const std::filesystem::path& specMapPath = "");

        Quad(const Quad&) = delete;
        Quad(Quad&&) noexcept = default;

        Quad& operator=(const Quad&) = delete;
        Quad& operator=(Quad&&) noexcept = default;

        void Draw(Graphics::ShaderProgram&);
    private:
        VBOBinder vbo{};
        VAOBinder vao{};
        //EBOBinder ebo{};
        TextureBinder texture, specTexture;

        constexpr static std::size_t cols{5};
        constexpr static std::size_t rows{6};

        constexpr static std::array<float, cols* rows> vertices{
          // positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
          0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
          0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
          1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

          0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
          1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
          1.0f,  0.5f,  0.0f,  1.0f,  0.0f
        };
    };
}