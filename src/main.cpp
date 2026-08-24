#include <glad/glad.h> // glad actually needs to be imported before glfw
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <initializer_list>
#include <math.h>

#include "WindowManager.h"
#include "shader.h"

float vertices[] = {
    // x, y, z, r, g, b
    0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // bottom left
    0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f // top
};

// Specify the order in which to connect vertices
unsigned int indices[] = {
    0, 1, 2
};

// Buffer objects
unsigned int EBO;   // Element Buffer Object
unsigned int VBO;   // Vertex Buffer Object
unsigned int VAO;   // Vertex Array Object

void processInput(GLFWwindow *window);

int main()
{
    // Handles boilerplate code (glfw, glad init)
    WindowManager& manager = WindowManager::getInstance();
    manager.init();
    GLFWwindow* window = manager.getWindow();

    // Generate VAO (stores buffer and attribute configurations for easily swapping between them)
    glGenVertexArrays(1, &VAO);

    // Bind it
    glBindVertexArray(VAO);

    // Generate Buffers
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind Buffers (subsequent calls on this target will affect the bound buffer object)
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Buffer the data

    // GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
    // GL_STATIC_DRAW: the data is set only once and used many times.
    // GL_DYNAMIC_DRAW: the data is changed a lot and used many times.

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Tell OpenGL how to interpret vertex data. You are basically describing the layout of your vertex data (which is in the buffer)
    // The buffer it pulls data from on depends on which VBO is bound to GL_ARRAY_BUFFER
    // In the vertex shader, data pulled from attribute zero will depend on what this function points to

    // attribute, size of attribute, data type, normalize t/f, stride, offset
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // attribute pointer for position data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // attribute pointer for color data

    // Enable attribute
    glEnableVertexAttribArray(0); 
    glEnableVertexAttribArray(1);

    // Unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Custom shader class to handle compilation and linking
    Shader shader("shaders/vertexShader.glsl", "shaders/fragmentShader.glsl");

    // Set clear color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Render Loop
    while(!glfwWindowShouldClose(window))
    {
        // Handle input
        processInput(window);

        // Clear buffer with the set clear color
        glClear(GL_COLOR_BUFFER_BIT);

        // Activate shader program
        shader.use();
        shader.setFloat("hOffset", 0.35f); // Set a float for the active shader program, so this goes after .use()

        // Render vertex data
        glBindVertexArray(VAO); // Bind VAO to use VBO and EBO data
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}