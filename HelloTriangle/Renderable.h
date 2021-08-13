#pragma once

#include "Camera.h"
#include "Lighting.h"
#include "Materials.h"
#include "Shader.h"
#include "Utilities.h"

#include <variant>

namespace Graphics
{
    struct RenderData
    {
        ShaderProgram shaderProgram;
        glm::mat4 model;
        std::vector<Lighting> lights;
        MaterialVariant material;
        float alphaThreshold;
    };

    template<class... RenderableVariants>
    class Renderable
    {
    public:
        template<class T>
        Renderable(T t, RenderData data)
            : m_Renderable(std::move(t))
            , m_Data{std::move(data)}
        {}

        void draw(const glm::mat4& view, const glm::mat4& projection, const camera& c)
        {
            updateUberPhong(m_Data.shaderProgram, view, projection, m_Data.model, m_Data.lights, m_Data.material, m_Data.alphaThreshold, c);
            std::visit(Utilities::overloaded{[&prog{m_Data.shaderProgram}] (auto& r) { r.Draw(prog); }}, m_Renderable);
        }
    private:
        std::variant<RenderableVariants...> m_Renderable;

        RenderData m_Data;
    };
}