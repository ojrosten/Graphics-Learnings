#pragma once

#include "glm/glm.hpp"
#include <variant>

namespace Graphics
{
    struct Material
    {
        glm::vec3 ambient{}, diffuse{}, specular{};
        float shininess{16};
    };

    struct MappedMaterial
    {
        float shininess{16};
        int diffuseIndex{}, specularIndex{1};
    };

    using MaterialVariant = std::variant<Material, MappedMaterial>;
}