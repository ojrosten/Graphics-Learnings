#pragma once

#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include <algorithm>

namespace Input
{

    void framebuffer_size_callback(GLFWwindow* window, int width, int height)
    {
        glViewport(0, 0, width, height);
    }

    class Mouse
    {
    public:
        static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
        {
            if (firstMouse)
            {
                lastX = xpos;
                lastY = ypos;
                firstMouse = false;
            }

            double xoffset = xpos - lastX;
            double yoffset = lastY - ypos; // reversed since y-coordinates range from bottom to top
            lastX = xpos;
            lastY = ypos;

            xoffset *= sensitivity;
            yoffset *= sensitivity;

            s_Yaw += xoffset;
            s_Pitch += yoffset;

            s_Pitch = std::clamp(s_Pitch, -maxAngle, maxAngle);
        }

        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
        {
            s_Zoom -= yoffset;
            s_Zoom = std::clamp(s_Zoom, 1.0, 45.0);
        }

        [[nodiscard]]
        static float yaw() { return static_cast<float>(s_Yaw); }

        [[nodiscard]]
        static float pitch() { return static_cast<float>(s_Pitch); }

        [[nodiscard]]
        static float zoom() { return static_cast<float>(s_Zoom); }
    private:
        inline static double lastX{ 400 }, lastY{ 300 }, s_Yaw{}, s_Pitch{}, s_Zoom{ 45 };

        constexpr static double maxAngle{ 89 };

        constexpr static double sensitivity{ 0.1 };

        inline static bool firstMouse{ true };
    };
}
