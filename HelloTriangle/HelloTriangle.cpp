
#include <array>

#include "Shader.h"
#include "Binder.h"
#include "Mesh.h"
#include "Input.h"
#include "InputProcessing.h"
#include "Model.h"
#include "Lighting.h"
#include "Examples/Cubes.h"
#include "Examples/Quad.h"

#include "glm/gtc/matrix_transform.hpp"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"




constexpr std::string_view fragmentShaderLightSource{ "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"    FragColor = vec4(1);\n"
"}" };

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    auto window{ glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL) };
    if (!window)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    glfwSetCursorPosCallback(window, Input::Mouse::mouse_callback);
    glfwSetScrollCallback(window, Input::Mouse::scroll_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, Input::framebuffer_size_callback);

    using namespace Graphics;

    ShaderProgram backpackShaderProgram{"../Shaders/model_loading.vs", "../Shaders/uber_phong.fs" };
    ShaderProgram texturedCubeShaderProgram{"../Shaders/cube.vs", "../Shaders/uber_phong.fs"};
    ShaderProgram lightSourceShaderProgram{"../Shaders/cube.vs", "../Shaders/uber_phong.fs"};
    ShaderProgram cubeShaderProgram{"../Shaders/cube.vs", "../Shaders/uber_phong.fs"};
    ShaderProgram scaledCubeShaderProgram{"../Shaders/cube.vs", "../Shaders/uber_phong.fs"};
    ShaderProgram grassShaderProgram{"../Shaders/quad.vs", "../Shaders/uber_phong.fs"};

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // Light source
    Lighting bottomLeftPointLight{{-2.0f, -2.0f, 4.0f}, {{0.2f, 0.2f, 0.2f}, {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}},
             topLeftPointLight{{-2.0f, 2.0f, 4.0f}, {{0.2f, 0.2f, 0.2f}, {0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}};
    Lighting directionalLight{{1.0, 1.0, 1.0}, {glm::vec3{1}, glm::vec3{}, glm::vec3{}}};
    //Lighting bottomLeftPointLight{{2.0f, 2.0f, 4.0f}, directional_light};

    stbi_set_flip_vertically_on_load(true);

    std::cout << std::filesystem::current_path() << '\n';

    Model backpack(std::filesystem::current_path().parent_path() / "Images" / "backpack" / "backpack.obj");
    Examples::Cube texturedCube{"C:/Users/oliver.rosten/source/repos/HelloTriangle/Images/container2.png",
                                "C:/Users/oliver.rosten/source/repos/HelloTriangle/Images/container2_specular.png"};
    Examples::Cube lightSourceCube{""}, cube{""}, scaledCube{""};
    Examples::Quad grass{"C:/Users/oliver.rosten/source/repos/HelloTriangle/Images/grass.png"};

    camera c{};
    Input::Mouse mouse{};
    double lastFrame{ glfwGetTime() };

    // Backpack
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1));

    // Textured Cube
    glm::mat4 texturedCubeModel = glm::mat4(1.0f);
    texturedCubeModel = glm::translate(texturedCubeModel, glm::vec3(-1.8f, -1.8f, 3.0f));
    texturedCubeModel = glm::scale(texturedCubeModel, glm::vec3(0.5));

    // Cube
    glm::mat4 cubeModel = glm::mat4(1.0f);
    cubeModel = glm::translate(cubeModel, glm::vec3(-2.0f, 1.0f, 2.0f));
    cubeModel = glm::scale(cubeModel, glm::vec3(0.5));

    // Scaled Cube
    glm::mat4 scaledCubeModel = glm::mat4(1.0f);
    scaledCubeModel = glm::translate(scaledCubeModel, glm::vec3(-2.0f, 1.0f, 2.0f));
    scaledCubeModel = glm::scale(scaledCubeModel, glm::vec3(0.52f));

    // Grass

    glm::mat4 grassModel = glm::mat4(1.0f);
    grassModel = glm::translate(grassModel, glm::vec3(-2.0f, 0, 2.5f));

    // Light Source
    const glm::vec3 lightSourcePos{bottomLeftPointLight.directionality()};
    glm::mat4 lightSourceModel = glm::mat4(1.0f);
    lightSourceModel = glm::translate(lightSourceModel, lightSourcePos); // translate it down so it's at the center of the scene
    lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.2f));

    MappedMaterial backpackMaterial{},
                   texturedCubeMaterial{32}, 
                   grassMaterial{};
    Material cubeMaterial{{0.3f, 0.7f, 0.3f}, {0.7f, 0.3f, 0.1f}, {0.2f, 0.1f, 0.1f}},
             scaledCubeMaterial{{1, 1, 0}, {}, {}},
             lightSourceMaterial{glm::vec3{1.0}, {}, {}, 0};

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    
    glClearColor(0.25f, 0.25f, 0.25f, 1.0f);

    while (!glfwWindowShouldClose(window))
    {
        const double currentFrame{ glfwGetTime() };
        const auto deltaTime{currentFrame - lastFrame};
        lastFrame = currentFrame;
        c = processInput(window, c, deltaTime, mouse);

        glStencilMask(0xFF);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glStencilMask(0x00);
        glEnable(GL_DEPTH_TEST);

        const auto view = glm::lookAt(c.pos, c.pos + c.front, c.up);
        const auto projection = glm::perspective(glm::radians(mouse.zoom()), 800.0f / 600.0f, 0.1f, 100.0f);

        // Light source
        updateUberPhong(lightSourceShaderProgram, view, projection, lightSourceModel, {directionalLight}, lightSourceMaterial, 0, c);
        lightSourceCube.Draw(lightSourceShaderProgram);

        // Textured Cube
        texturedCubeModel = glm::rotate(texturedCubeModel, fmodf(static_cast<float>(glm::radians(10.0f) * deltaTime), static_cast<float>(glm::radians(360.0))), glm::vec3(0.0f, 1.0f, 0.0f));
        updateUberPhong(texturedCubeShaderProgram, view, projection, texturedCubeModel, {bottomLeftPointLight, topLeftPointLight}, texturedCubeMaterial, 0, c);
        texturedCube.Draw(texturedCubeShaderProgram);

        // Backpack
        model = glm::rotate(model, fmodf(static_cast<float>(glm::radians(10.0f) * deltaTime), static_cast<float>(glm::radians(360.0))), glm::vec3(0.0f, 1.0f, 0.0f));
        updateUberPhong(backpackShaderProgram, view, projection, model, {bottomLeftPointLight, topLeftPointLight}, {backpackMaterial}, 0, c);
        backpack.Draw(backpackShaderProgram);

        // Grass
        updateUberPhong(grassShaderProgram, view, projection, grassModel, {bottomLeftPointLight, topLeftPointLight}, grassMaterial, 0.1f, c);
        grass.Draw(grassShaderProgram);


        glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
        glStencilMask(0xFF); // enable writing to the stencil buffer;
        // Cube
        updateUberPhong(cubeShaderProgram, view, projection, cubeModel, {bottomLeftPointLight, topLeftPointLight}, cubeMaterial, 0, c);
        cube.Draw(cubeShaderProgram);

        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00); // disable writing to the stencil buffer
        glDisable(GL_DEPTH_TEST);

        // Scaled cube
        updateUberPhong(scaledCubeShaderProgram, view, projection, scaledCubeModel, {bottomLeftPointLight, topLeftPointLight}, scaledCubeMaterial, 0, c);
        scaledCube.Draw(scaledCubeShaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
