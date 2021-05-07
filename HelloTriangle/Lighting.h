#pragma once

#include "glm/glm.hpp"
#include <type_traits>

namespace Graphics
{
    class Lighting
    {
    public:
        enum class light_flavour { directional, point };

        using directional_t = std::integral_constant<light_flavour, light_flavour::directional>;
        using point_t = std::integral_constant<light_flavour, light_flavour::point>;

        Lighting(glm::vec3 direction, directional_t)
            : m_Directionality{direction, 0}
        {}


        Lighting(glm::vec3 position, point_t)
            : m_Directionality{position, 1}
        {}

        [[nodiscard]]
        const glm::vec4& directionality() const noexcept
        {
            return m_Directionality;
        }

        [[nodiscard]]
        const glm::vec3& ambient() const noexcept
        {
            return m_Ambient;
        }

        [[nodiscard]]
        const glm::vec3& diffuse() const noexcept
        {
            return m_Diffuse;
        }

        [[nodiscard]]
        const glm::vec3& specular() const noexcept
        {
            return m_Specular;
        }

        [[nodiscard]]
        float cos_inner_cutoff() const noexcept
        {
            return m_InnerCutoff;
        }
   
        [[nodiscard]]
        float cos_outer_cutoff() const noexcept
        {
            return m_OuterCutoff;
        }
    private:
        glm::vec4 m_Directionality;

        glm::vec3 m_Ambient{0.2f, 0.2f, 0.2f},
                  m_Diffuse{0.5f, 0.5f, 0.5f},
                  m_Specular{1.0f, 1.0f, 1.0f};

        float m_InnerCutoff{glm::cos(glm::radians(12.0f))}, m_OuterCutoff{glm::cos(glm::radians(17.0f))};
    };

    inline constexpr Lighting::point_t point_light{};
    inline constexpr Lighting::directional_t directional_light{};
}