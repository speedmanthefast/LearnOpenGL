#include "WindowManager.h"
#include <iostream>

bool WindowManager::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Initialzie GLFW: Handles creating windows
    m_window = glfwCreateWindow(m_width, m_height, "LearnOpenGL", NULL, NULL);
    if (m_window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(m_window);

    // User pointer to access app singleton in C-style function pointers / lambdas
    glfwSetWindowUserPointer(m_window, this);

    // Initialize GLAD: Links OpenGL functions to graphics driver functions
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }

    // Set viewport size
    glViewport(0, 0, m_width, m_height);

    // Set resizing callback function
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