#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>

#include "config.h"

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Initialzie GLFW: Handles creating windows
    GLFWwindow* window = glfwCreateWindow(windowSettings.width, windowSettings.height, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize GLAD: Links OpenGL functions to graphics driver functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set viewport size
    glViewport(0, 0, windowSettings.width, windowSettings.height);

    // Set resizing callback function
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow*, int width, int height)
    {
        windowSettings.width = width;
        windowSettings.height = height;

        glViewport(0, 0, windowSettings.width, windowSettings.height);
        // glViewport(0, 0, 800, 600);
    });

    // Render Loop
    while(!glfwWindowShouldClose(window))
    {
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}