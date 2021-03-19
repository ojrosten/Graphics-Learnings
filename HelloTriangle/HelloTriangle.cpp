
#include <array>

#include "Shader.h"
#include "Binder.h"
#include "Mesh.h"
#include "Input.h"
#include "InputProcessing.h"
#include "Model.h"
#include "Examples/Cubes.h"

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

    ShaderProgram backpackShaderProgram{"../Shaders/model_loading.vs", "../Shaders/phong_with_texture.fs" };
    ShaderProgram texturedCubeShaderProgram{"../Shaders/textured_cube.vs", "../Shaders/phong_with_maps.fs"};
    ShaderProgram lightSourceShaderProgram{"../Shaders/cube.vs", "../Shaders/phong.fs"};
    ShaderProgram cubeShaderProgram{"../Shaders/cube.vs", "../Shaders/phong.fs"};

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    backpackShaderProgram.use();
    backpackShaderProgram.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
    backpackShaderProgram.setFloat("ambientStrength", 0.0f);
    backpackShaderProgram.setFloat("specularStrength", 0.5f);
    backpackShaderProgram.setFloat("shininess", 32);

    texturedCubeShaderProgram.use();
    texturedCubeShaderProgram.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    texturedCubeShaderProgram.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    texturedCubeShaderProgram.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    texturedCubeShaderProgram.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    texturedCubeShaderProgram.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    texturedCubeShaderProgram.setInt("material.diffuse", 0);
    texturedCubeShaderProgram.setFloat("material.shininess", 32);

    cubeShaderProgram.use();
    cubeShaderProgram.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    cubeShaderProgram.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    cubeShaderProgram.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    cubeShaderProgram.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
    cubeShaderProgram.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
    cubeShaderProgram.setVec3("material.diffuse", 0.3f, 0.15f, 0.2f);
    cubeShaderProgram.setFloat("material.shininess", 32);

    lightSourceShaderProgram.use();
    lightSourceShaderProgram.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
    lightSourceShaderProgram.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    lightSourceShaderProgram.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
    lightSourceShaderProgram.setVec3("material.ambient", glm::vec3{1.0f});
    lightSourceShaderProgram.setVec3("material.specular", glm::vec3{1.0f});
    lightSourceShaderProgram.setVec3("material.diffuse", glm::vec3{1.0f});
    lightSourceShaderProgram.setFloat("material.shininess", 0);

    stbi_set_flip_vertically_on_load(true);
    glEnable(GL_DEPTH_TEST);

    std::cout << std::filesystem::current_path() << '\n';

    Model ourModel(std::filesystem::current_path().parent_path() / "Images" / "backpack" / "backpack.obj");
    Examples::Cube texturedCube{"C:/Users/oliver.rosten/source/repos/HelloTriangle/Images/container.jpg"};
    Examples::Cube lightSourceCube{""}, cube{""};


    camera c{};
    Input::Mouse mouse{};
    double lastFrame{ glfwGetTime() };

    // Backpack
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
    model = glm::scale(model, glm::vec3(1));

    // Textured Cube
    glm::mat4 texturedCubeModel = glm::mat4(1.0f);
    texturedCubeModel = glm::translate(texturedCubeModel, glm::vec3(2.0f, 1.0f, 2.0f));
    texturedCubeModel = glm::scale(texturedCubeModel, glm::vec3(0.5));

    // Cube
    glm::mat4 cubeModel = glm::mat4(1.0f);
    cubeModel = glm::translate(cubeModel, glm::vec3(-2.0f, 1.0f, 2.0f));
    cubeModel = glm::scale(cubeModel, glm::vec3(0.5));

    // Light source
    const glm::vec3 lightSourcePos{-2.0f, -2.0f, 4.0f};
    glm::mat4 lightSourceModel = glm::mat4(1.0f);
    lightSourceModel = glm::translate(lightSourceModel, lightSourcePos); // translate it down so it's at the center of the scene
    lightSourceModel = glm::scale(lightSourceModel, glm::vec3(0.2f));

    backpackShaderProgram.setVec3("lightPos", lightSourcePos);

    while (!glfwWindowShouldClose(window))
    {
        const double currentFrame{ glfwGetTime() };
        const auto deltaTime{currentFrame - lastFrame};
        lastFrame = currentFrame;
        c = processInput(window, c, deltaTime, mouse);

        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        const auto view = glm::lookAt(c.pos, c.pos + c.front, c.up);
        const auto projection = glm::perspective(glm::radians(mouse.zoom()), 800.0f / 600.0f, 0.1f, 100.0f);
        model = glm::rotate(model, fmodf(static_cast<float>(glm::radians(10.0f) * deltaTime), static_cast<float>(glm::radians(360.0))), glm::vec3(0.0f, 1.0f, 0.0f));

        // Textured Cube
        texturedCubeShaderProgram.use();
        texturedCubeShaderProgram.setMatrix4("view", view);
        texturedCubeShaderProgram.setMatrix4("projection", projection);
        texturedCubeShaderProgram.setMatrix4("model", texturedCubeModel);
        texturedCubeShaderProgram.setVec3("lightPos", lightSourcePos);
        texturedCubeShaderProgram.setVec3("viewPos", c.pos);

        texturedCube.Draw(texturedCubeShaderProgram);

        // Cube
        cubeShaderProgram.use();
        cubeShaderProgram.setMatrix4("view", view);
        cubeShaderProgram.setMatrix4("projection", projection);
        cubeShaderProgram.setMatrix4("model", cubeModel);
        cubeShaderProgram.setVec3("light.position", lightSourcePos);
        cubeShaderProgram.setVec3("viewPos", c.pos);

        cube.Draw(cubeShaderProgram);

        // Light source
        lightSourceShaderProgram.use();
        lightSourceShaderProgram.setMatrix4("view", view);
        lightSourceShaderProgram.setMatrix4("projection", projection);
        lightSourceShaderProgram.setMatrix4("model", lightSourceModel);

        lightSourceCube.Draw(lightSourceShaderProgram);

        // Backpack
        backpackShaderProgram.use();
        backpackShaderProgram.setMatrix4("view", view);
        backpackShaderProgram.setMatrix4("projection", projection);
        backpackShaderProgram.setMatrix4("model", model);
        backpackShaderProgram.setVec3("lightPos", lightSourcePos);
        backpackShaderProgram.setVec3("viewPos", c.pos);

        ourModel.Draw(backpackShaderProgram);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
