#pragma once

#include "glm/glm.hpp"
#include <type_traits>

namespace Graphics
{
    struct Attenuation
    {
        float constant{1.0},
              linear{0.09f},
              quadratic{0.032f};
    };

    struct LightingComponents
    {
        glm::vec3 ambient{0.2f, 0.2f, 0.2f},
                  diffuse{0.5f, 0.5f, 0.5f},
                  specular{1.0f, 1.0f, 1.0f};
    };

    struct LightShape
    {
        float cosInnerCutoff{glm::cos(glm::radians(12.0f))},
              cosOuterCutoff{glm::cos(glm::radians(17.0f))};
    };

    class Lighting
    {
    public:
        Lighting(glm::vec3 direction, LightingComponents components)
            : m_Directionality{direction, 1}
            , m_Components{std::move(components)}
        {}


        Lighting(glm::vec3 position, LightingComponents components, LightShape shape, Attenuation att)
            : m_Directionality{position, 0}
            , m_Components{std::move(components)}
            , m_Shape{std::move(shape)}
            , m_Attenuation{std::move(att)}
        {}

        [[nodiscard]]
        const glm::vec4& directionality() const noexcept
        {
            return m_Directionality;
        }

        [[nodiscard]]
        const LightingComponents& components() const noexcept
        {
            return m_Components;
        }
        [[nodiscard]]
        const LightShape& shape() const noexcept
        {
            return m_Shape;
        }

        [[nodiscard]]
        const Attenuation& attenuation() const noexcept
        {
            return m_Attenuation;
        }
    private:
        glm::vec4 m_Directionality;

        LightingComponents m_Components{};

        LightShape m_Shape{};

        Attenuation m_Attenuation{};
    };
}