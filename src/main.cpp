#include <E:/C++PRJ/battle-city/external/glad/include/glad/glad.h>
#include <E:/C++PRJ/battle-city/external/glfw/include/GLFW/glfw3.h>
#include <glm/vec2.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <chrono>

#include "Renderer/ShaderProgram.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Texture2D.h"
#include "Renderer/Sprite.h"
#include "Renderer/AnimatedSprite.h"

GLfloat point[] = {
    0.0f, 100.f, 0.0f,
    100.f, -100.f, 0.0f,
    -100.f, -100.f, 0.0f};

GLfloat colors[] = {
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f

};

GLfloat texCoord[] = {
    0.5f,
    1.0f,
    1.0f,
    0.0f,
    0.0f,
    0.0f

};

glm::ivec2 g_WindowSize(640, 480);

bool isEagle = false;
void glfwWindowSizeCallBack(GLFWwindow *pWindow, int width, int height)
{
    g_WindowSize.x = width;
    g_WindowSize.y = height;
    glViewport(0, 0, width, height);
}

void glfwKeyCallBack(GLFWwindow *pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    if (key == GLFW_KEY_ENTER && action == GLFW_PRESS)
    {
        isEagle = !isEagle;
    }
}
int main(int argc, char **argv)
{

    GLFWwindow *PWWindow;

    /* Initialize the library */
    if (!glfwInit())
    {
        std::cout << "glfwInit Failed" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    PWWindow = glfwCreateWindow(g_WindowSize.x, g_WindowSize.y, "Radforga City", NULL, NULL);
    if (!PWWindow)
    {
        std::cout << "glfwCreateWindow Failed" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwSetWindowSizeCallback(PWWindow, glfwWindowSizeCallBack);
    glfwSetKeyCallback(PWWindow, glfwKeyCallBack);

    /* Make the window's context current */
    glfwMakeContextCurrent(PWWindow);

    if (!gladLoadGL())
    {
        std::cout << "ERROR" << std::endl;
        return -1;
    }

    std::cout << "Renderer " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "OpenGL Version " << glGetString(GL_VERSION) << std::endl;

    std::cout << "Opengl " << GLVersion.major << "." << GLVersion.minor << std::endl;

    glClearColor(0.6, 1, 0.6, 1);

    {
        ResourceManager resourceManager(argv[0]);
        auto pDefaultShaderProgram = resourceManager.loadShaders("DefaultShader", "res/shaders/vertex.txt", "res/shaders/fragment.txt");
        if (!pDefaultShaderProgram)
        {
            std::cerr << "Can't create program: DefaultShader" << std::endl;
            return -1;
        }

        auto pSpriteShaderProgram = resourceManager.loadShaders("SpriteShader", "res/shaders/vSprite.txt", "res/shaders/fSprite.txt");
        if (!pSpriteShaderProgram)
        {
            std::cerr << "Can't create program: SpriteShader" << std::endl;
            return -1;
        }

        auto tex = resourceManager.loadTextures("DefaultTexture", "res/textures/map_16x16.png");

        std::vector<std::string> SubTexturesName = {"BrickWall",
                                                    "topBrick",
                                                    "botBrick",
                                                    "leftBrick",
                                                    "rightBrick",
                                                    "topLeftBrick",
                                                    "topRightBrick",
                                                    "BotLeftBrick",
                                                    "botRightBrick",
                                                    "IronWall",
                                                    "topIronWall",
                                                    "bottomIronWall",
                                                    "leftIronWall",
                                                    "rightIronWall",
                                                    "topLeftIronWall",
                                                    "topRightIronWall",

                                                    "bottomLeftIronWall",
                                                    "bottomRightIronWall",
                                                    "water1",
                                                    "water2",
                                                    "water3",
                                                    "trees",
                                                    "ice",
                                                    "wall",

                                                    "eagle",
                                                    "deadEagle",
                                                    "nothing",
                                                    "respawn1",
                                                    "respawn2",
                                                    "respawn3",
                                                    "respawn4"};
        auto pTextureAtlas = resourceManager.loadTextureAtlas("DefaultTextureAtlas", "res/textures/map_16x16.png", std::move(SubTexturesName), 16, 16);

        auto pSprite = resourceManager.loadSprite("NewSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, 0, "IronWall");
        pSprite->setPosition(glm::vec2(250, 100));

        auto pAnimatedSprite = resourceManager.loadAnimatedSprite("NewAnimatedSprite", "DefaultTextureAtlas", "SpriteShader", 100, 100, 0, "IronWall");
        pAnimatedSprite->setPosition(glm::vec2(300, 300));
        std::vector<std::pair<std::string, uint64_t>> waterState;
        waterState.emplace_back(std::pair<std::string, uint64_t>("water1", 1000000000));
        waterState.emplace_back(std::pair<std::string, uint64_t>("water2", 1000000000));
        waterState.emplace_back(std::pair<std::string, uint64_t>("water3", 1000000000));

        std::vector<std::pair<std::string, uint64_t>> eagleState;
        eagleState.emplace_back(std::pair<std::string, uint64_t>("eagle", 1000000000));
        eagleState.emplace_back(std::pair<std::string, uint64_t>("deadEagle", 1000000000));

        pAnimatedSprite->insertState("waterState", std::move(waterState));
        pAnimatedSprite->insertState("eagleState", std::move(eagleState));

        pAnimatedSprite->setState("waterState");

        GLuint points_vbo = 0;
        glGenBuffers(1, &points_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(point), point, GL_STATIC_DRAW);

        GLuint colors_vbo = 0;
        glGenBuffers(1, &colors_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

        GLuint texture_vbo = 0;
        glGenBuffers(1, &texture_vbo);
        glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(texCoord), texCoord, GL_STATIC_DRAW);

        GLuint vao = 0;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, points_vbo);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colors_vbo);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, texture_vbo);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

        pDefaultShaderProgram->use();
        pDefaultShaderProgram->setInt("tex", 0);

        glm::mat4 modelMatrix_1 = glm::mat4(1.f);
        modelMatrix_1 = glm::translate(modelMatrix_1, glm::vec3(100.f, 200.f, 0.f));

        glm::mat4 modelMatrix_2 = glm::mat4(1.f);
        modelMatrix_2 = glm::translate(modelMatrix_2, glm::vec3(540.f, 200.f, 0.f));

        glm::mat4 projectionMatrix = glm::ortho(0.f, static_cast<float>(g_WindowSize.x), 0.f, static_cast<float>(g_WindowSize.y), -100.f, 100.f);

        pDefaultShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        pSpriteShaderProgram->use();
        pSpriteShaderProgram->setInt("tex", 0);
        pSpriteShaderProgram->setMatrix4("projectionMat", projectionMatrix);

        auto lastTime = std::chrono::high_resolution_clock::now();
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(PWWindow))
        {
            if (isEagle)
            {
                pAnimatedSprite->setState("eagleState");
            }
            else
            {
                pAnimatedSprite->setState("waterState");
            }

            auto curentTime = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(curentTime - lastTime).count();
            lastTime = curentTime;

            pAnimatedSprite->update(duration);

            /* Render here */
            glClear(GL_COLOR_BUFFER_BIT);

            pDefaultShaderProgram->use();
            glBindVertexArray(vao);
            tex->bind();

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_1);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pDefaultShaderProgram->setMatrix4("modelMat", modelMatrix_2);
            glDrawArrays(GL_TRIANGLES, 0, 3);

            pSprite->render();

            pAnimatedSprite->render();

            /* Swap front and back buffers */
            glfwSwapBuffers(PWWindow);

            /* Poll for and process events */
            glfwPollEvents();
        }
    }
    glfwTerminate();
    return 0;
}