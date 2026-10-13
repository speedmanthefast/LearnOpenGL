#include <glad/glad.h> // glad actually needs to be imported before glfw
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <initializer_list>
#include <math.h>

#include "WindowManager.h"
#include "shader.h"
#include "stb_image.h"
#include "texture.h"

float vertices[] = {
    // positions        // colors           // texture coords
    0.5f, 0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
    0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
};

// Specify the order in which to connect vertices
unsigned int indices[] = {
    0, 2, 3, // first triangle  (top-right, bottom-left, top-left)
    0, 1, 2  // second triangle (top-right, bottom-right, bottom-left)
};

// Store texture data
Texture tex1, tex2;
float texBlend = 0.5f;
float keyCooldown = 1.0f;


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

    // Generate texture
    glGenTextures(1, &tex1.id);

    // Bind Texture
    glActiveTexture(GL_TEXTURE0); // activate texture unit, subsequent binds hit this unit
    glBindTexture(GL_TEXTURE_2D, tex1.id); 

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load texture data
    stbi_set_flip_vertically_on_load(true);
    tex1.data = stbi_load("./assets/stone.png", &tex1.width, &tex1.height, &tex1.nrChannels, 0);

    if (tex1.data)
    {
        // Generate texture
        // Args: texture target, corresponding mipmap level, resulting texture format, texture width, texture height, legacy stuff (this should always be zero), source data format, source data datatype, source data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex1.width, tex1.height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1.data);

        // To use mipmaps, you could call the previous function again and set each mipmap manually for each level, or you could generate them manually with
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(tex1.data);


    // Generate texture
    glGenTextures(1, &tex2.id);

    // Bind Texture
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, tex2.id);

    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Load texture data
    tex2.data = stbi_load("./assets/critic.png", &tex2.width, &tex2.height, &tex2.nrChannels, 0);

    if (tex2.data)
    {
        // Generate texture
        // Args: texture target, corresponding mipmap level, resulting texture format, texture width, texture height, legacy stuff (this should always be zero), source data format, source data datatype, source data
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex2.width, tex2.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex2.data);

        // To use mipmaps, you could call the previous function again and set each mipmap manually for each level, or you could generate them manually with
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(tex2.data);

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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0); // attribute pointer for position data
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float))); // attribute pointer for color data
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float))); // attribute pointer for texture coord data (2 components: s, t)

    // Enable attribute
    glEnableVertexAttribArray(0); 
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

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

        // Assign texture units to frag shader uniforms
        glUniform1i(glGetUniformLocation(shader.ID, "texture1"), 0); // manually
        shader.setInt("texture2", 1); // or with shader class
        shader.setFloat("blend", texBlend);

        // Render vertex data
        glBindVertexArray(VAO); // Bind VAO to use VBO and EBO data
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // mode, number of indicies, data type, offset
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
    if (keyCooldown > 0)
    {
        keyCooldown = keyCooldown - 0.01f;
        return;
    }

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        keyCooldown = 1.0f;
        texBlend = texBlend + 0.05f;
        if (texBlend > 1)
            texBlend = 1;
    }

    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        keyCooldown = 1.0f;
        texBlend = texBlend - 0.05f;
        if (texBlend < 0)
            texBlend = 0;
    }
}