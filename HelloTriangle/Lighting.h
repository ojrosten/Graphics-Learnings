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
    private:
        glm::vec4 m_Directionality;
    };

    inline constexpr Lighting::point_t point_light{};
    inline constexpr Lighting::directional_t directional_light{};
}