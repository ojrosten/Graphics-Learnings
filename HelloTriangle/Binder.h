#pragma once


#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "stb_image.h"

#include <string>
#include <filesystem>
#include <iostream>

namespace Graphics
{
    class VBOBinder
    {
        unsigned int m_VBO{};
    public:
        VBOBinder()
        {
            glGenBuffers(1, &m_VBO);
            bind();
        }

        VBOBinder(VBOBinder&& other) noexcept
            : m_VBO{std::exchange(other.m_VBO, 0)}
        {}

        VBOBinder& operator=(VBOBinder&& other) noexcept
        {
            m_VBO = std::exchange(other.m_VBO, 0);
        }

        ~VBOBinder()
        {
            if(m_VBO) glDeleteBuffers(1, &m_VBO);
        }

        [[nodiscard]]
        unsigned int get() const noexcept
        {
            return m_VBO;
        }

        void bind()
        {
          glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        }
    };

    class VAOBinder
    {
        unsigned int m_VAO{};
    public:
        VAOBinder()
        {
            glGenVertexArrays(1, &m_VAO);
            bind();
        }

        VAOBinder(VAOBinder&& other) noexcept
            : m_VAO{std::exchange(other.m_VAO, 0)}
        {}

        VAOBinder& operator=(VAOBinder&& other) noexcept
        {
            m_VAO = std::exchange(other.m_VAO, 0);
        }

        ~VAOBinder()
        {
            if(m_VAO) glDeleteVertexArrays(1, &m_VAO);
        }

        [[nodiscard]]
        unsigned int get() const noexcept
        {
            return m_VAO;
        }

        void bind()
        {
          glBindVertexArray(m_VAO);
        }
    };

    class EBOBinder
    {
        unsigned int m_EBO{};
    public:
        EBOBinder()
        {
            glGenBuffers(1, &m_EBO);
            bind();
        }

        EBOBinder(EBOBinder&& other) noexcept
            : m_EBO{std::exchange(other.m_EBO, 0)}
        {}

        EBOBinder& operator=(EBOBinder&& other) noexcept
        {
            m_EBO = std::exchange(other.m_EBO, 0);
        }

        ~EBOBinder()
        {
            if(m_EBO) glDeleteBuffers(1, &m_EBO);
        }

        [[nodiscard]]
        unsigned int get() const noexcept
        {
            return m_EBO;
        }

        void bind()
        {
          glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
        }
    };

    class TextureBinder
    {
        unsigned int m_Texture{};
        int m_gl_texture{};
    public:
        TextureBinder(const int gl_texture, const std::filesystem::path& file, const bool verticalFlip = false)
          : m_gl_texture{gl_texture}
        {
            glGenTextures(1, &m_Texture);
            bind();

            load(file, verticalFlip);
        }

        TextureBinder(TextureBinder&& other) noexcept
          : m_Texture{std::exchange(other.m_Texture, 0)}
        {}

        TextureBinder& operator=(TextureBinder&& other) noexcept
        {
            m_Texture = std::exchange(other.m_Texture, 0);
        }

        ~TextureBinder()
        {
            glDeleteTextures(1, &m_Texture);
        }

        [[nodiscard]]
        unsigned int get() const noexcept
        {
            return m_Texture;
        }

        void bind()
        {
          glActiveTexture(m_gl_texture);
          glBindTexture(GL_TEXTURE_2D, m_Texture);
        }

        void load(const std::filesystem::path& file, const bool verticalFlip = false)
        {
            if(file.empty()) return;

            stbi_set_flip_vertically_on_load(verticalFlip);
            if (int width, height, nrChannels; unsigned char* data = stbi_load(file.string().data(), &width, &height, &nrChannels, 0))
            {
                GLenum format{};
                switch (nrChannels)
                {
                case 1:
                    format = GL_RED;
                    break;
                case 3:
                    format = GL_RGB;
                    break;
                default:
                    format = GL_RGBA;
                }

                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                stbi_image_free(data);
            }
            else
            {
                std::cout << "Failed to load texture" << '\n';
            }
        }

        static void initParameters()
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        }
    };
}