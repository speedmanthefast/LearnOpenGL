#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <initializer_list>
#include <math.h>

#include "WindowManager.h"

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 ourColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = ourColor;\n"
    "}\0";

float vertices[] = {
    0.5f, 0.5f, 0.0f, // top right
    0.5f, -0.5f, 0.0f, // bottom right
    -0.5f, -0.5f, 0.0f, // bottom left
    -0.5f, 0.5f, 0.0f // top left
};

unsigned int indices[] = {
    0, 1, 3, // first triangle
    1, 2, 3 // second triangle
};
unsigned int EBO;

unsigned int VBO;
unsigned int VAO;

void processInput(GLFWwindow *window);
unsigned int compileShader(const char* shaderSource, int type);
unsigned int createShaderProgram(std::initializer_list<unsigned int> list);

int main()
{
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0); // attribute, size of attribute, data type, normalize t/f, stride, offset

    // Enable attribute
    glEnableVertexAttribArray(0); 

    // Unbind buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    // Create vertex shader
    unsigned int vertexShader = compileShader(vertexShaderSource, GL_VERTEX_SHADER);

    // Create fragment shader
    unsigned int fragmentShader = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    unsigned int shaderProgram = createShaderProgram({vertexShader, fragmentShader});

    // Set clear color
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    // Render Loop
    while(!glfwWindowShouldClose(window))
    {
        // Handle input
        processInput(window);

        // Clear buffer with the set clear color
        glClear(GL_COLOR_BUFFER_BIT);

        // Calculate color for this frame
        float timeValue = glfwGetTime();
        float greenValue = (std::sin(timeValue) / 2.0f) + 0.5f;

        // Query the location of the 'outColor' uniform
        int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");

        // Activate shader program
        glUseProgram(shaderProgram);

        // Load color data into uniform. Need to call glUseProgram first because this function operates on the currently bound shader program.
        glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f); 

        // Render vertex data
        glBindVertexArray(VAO); // Bind VAO to use VBO and EBO data
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

unsigned int compileShader(const char* shaderSource, int type)
{
    // Create vertex shader
    unsigned int shaderID = glCreateShader(type); // Create shader object
    glShaderSource(shaderID, 1, &shaderSource, NULL);
    glCompileShader(shaderID);

    // Check if compilation succeeded
    int success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    
    // Print info if not
    char infoLog[512];
    if (!success)
    {
        std::string shaderTypeString;
        if (type == GL_VERTEX_SHADER) { shaderTypeString = "VERTEX"; }
        else if (type == GL_FRAGMENT_SHADER ) { shaderTypeString = "FRAGMENT"; }

        glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << shaderTypeString << "::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    return shaderID;
}

unsigned int createShaderProgram(std::initializer_list<unsigned int> list)
{
    // Create shader program. This links shaders together into a proper pipeline.
    unsigned int shaderProgram = glCreateProgram();
    for (int shader : list)
    {
        glAttachShader(shaderProgram, shader);
    }

    glLinkProgram(shaderProgram); // this is what connects outputs to inputs of the shaders in the pipeline

    // Check for errors
    int success;
    char infoLog[512];

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINK_FAILED\n" << infoLog << std::endl;
    }

    // Clean up shader objects
    for (int shader : list)
    {
        glDeleteShader(shader);
    }

    return shaderProgram;
}