#pragma once

#include "glm/glm.hpp"

namespace Graphics
{
    struct camera
    {
        glm::vec3 pos = glm::vec3(0.0f, 0.0f, 10.0f);
        glm::vec3 front = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        float speed = 0.01f;
        float multiplier{1};
    };
}