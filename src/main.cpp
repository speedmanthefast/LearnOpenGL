#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <initializer_list>

#include "config.h"

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2, 1.0f);\n"
    "}\0";

float vertices1[] = {
    0.5f, 0.5f, 0.0f, // top 
    0.75f, -0.5f, 0.0f, // bottom right
    0.25f, -0.5f, 0.0f // bottom left
};

float vertices2[] = {
    -0.5f, 0.5f, 0.0f, // top
    -0.25f, -0.5f, 0.0f, // bottom right
    -0.75f, -0.5f, 0.0f // bottom left
};

unsigned int indices[] = {
    0, 1, 2, // works for both triangles as long as their corresponding vertices are buffered in the same order
};

unsigned int EBO;
unsigned int VBO1;
unsigned int VBO2;
unsigned int VAO1;
unsigned int VAO2;

void processInput(GLFWwindow *window);
unsigned int compileShader(const char* shaderSource, int type);
unsigned int createShaderProgram(std::initializer_list<unsigned int> list);

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
    });

    // Generate Buffers
    glGenVertexArrays(1, &VAO1); // stores vertex buffer and attribute configuration
    glGenVertexArrays(1, &VAO2);
    glGenBuffers(1, &VBO1); // stores vertex data raw
    glGenBuffers(1, &VBO2);
    glGenBuffers(1, &EBO); // stores indices of vertex data

    // Bind it
    glBindVertexArray(VAO1);

    // Bind Buffers (subsequent calls on this target will affect the bound buffer object)
    glBindBuffer(GL_ARRAY_BUFFER, VBO1);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Buffer the data

    // GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
    // GL_STATIC_DRAW: the data is set only once and used many times.
    // GL_DYNAMIC_DRAW: the data is changed a lot and used many times.

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1), vertices1, GL_STATIC_DRAW);
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

    // Do it again but for second triangle

    // Bind it
    glBindVertexArray(VAO2);

    // Bind Buffers (subsequent calls on this target will affect the bound buffer object)
    glBindBuffer(GL_ARRAY_BUFFER, VBO2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    // Buffer the data

    // GL_STREAM_DRAW: the data is set only once and used by the GPU at most a few times.
    // GL_STATIC_DRAW: the data is set only once and used many times.
    // GL_DYNAMIC_DRAW: the data is changed a lot and used many times.

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STATIC_DRAW);
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
        processInput(window);

        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);
        glBindVertexArray(VAO1);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(VAO2);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwPollEvents();
        glfwSwapBuffers(window);
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