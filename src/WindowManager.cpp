#include "WindowManager.h"
#include <iostream>

bool WindowManager::init()
{
    // GLFW Initialization
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create GLFW Window
    m_window = glfwCreateWindow(m_width, m_height, "LearnOpenGL", NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }

    // Subsqeuent calls to GLFW will affect this window
    glfwMakeContextCurrent(m_window);

    // Set a 'user pointer' to reference app singleton in C-style function pointers / lambdas
    glfwSetWindowUserPointer(m_window, this);

    // Initialize GLAD: Links C++ functions to graphics driver functions for OpenGL
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // Set viewport size
    glViewport(0, 0, m_width, m_height);

    // Set resizing callback function. This will be called any time you resize the window.
    glfwSetFramebufferSizeCallback(m_window, [](GLFWwindow* win, int width, int height)
    {

        WindowManager* manager = static_cast<WindowManager*>(glfwGetWindowUserPointer(win));

        manager->m_width = width;
        manager->m_height = height;

        glViewport(0, 0, width, height);
    });

    return true;
}

GLFWwindow* WindowManager::getWindow()
{
    return m_window;
}

int WindowManager::getMaxAttributes() const
{
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    return nrAttributes;
}