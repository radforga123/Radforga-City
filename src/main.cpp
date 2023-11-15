#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/vec2.hpp>

#include "Game/Game.h"
#include "Resources/ResourceManager.h"
#include "Renderer/Renderer.h"

#include <iostream>
#include <chrono>

glm::ivec2 g_WindowSize(13 * 16, 14 * 16);
std::unique_ptr<Game> g_Game = std::make_unique<Game>(g_WindowSize);

void glfwWindowSizeCallBack(GLFWwindow *pWindow, int width, int height)
{
    g_WindowSize.x = width;
    g_WindowSize.y = height;
    RenderEngine::Renderer::setViewport(width, height);
}

void glfwKeyCallBack(GLFWwindow *pWindow, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GL_TRUE);
    }
    g_Game->setKey(key, action);
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

    std::cout << "Renderer " << RenderEngine::Renderer::getRendererSTR << std::endl;
    std::cout << "OpenGL Version " << RenderEngine::Renderer::getVersionSTR << std::endl;

    std::cout << "Opengl " << GLVersion.major << "." << GLVersion.minor << std::endl;

    RenderEngine::Renderer::setClearColor(0, 0, 0, 1);

    {
        ResourceManager::setExecutablePath(argv[0]);
        g_Game->init();

        auto lastTime = std::chrono::high_resolution_clock::now();
        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(PWWindow))
        {

            /* Poll for and process events */
            glfwPollEvents();

            auto curentTime = std::chrono::high_resolution_clock::now();
            uint64_t duration = std::chrono::duration_cast<std::chrono::nanoseconds>(curentTime - lastTime).count();
            lastTime = curentTime;
            g_Game->update(duration);

            /* Render here */
            RenderEngine::Renderer::clear();

            g_Game->render();
            /* Swap front and back buffers */
            glfwSwapBuffers(PWWindow);
        }
        g_Game = nullptr;
        ResourceManager::unloadAllRecources();
    }
    glfwTerminate();
    return 0;
}