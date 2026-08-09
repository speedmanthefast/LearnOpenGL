#pragma once

#include <glad/glad.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Shader
{
    public:

        // Program ID
        unsigned int ID;

        Shader(const char* vertexPath, const char* fragmentPath); // Constructor that reads and builds the shader
        void use(); // use/activate the shader

        // Utility uniform functions
        // Declare these const even though they are setters because they modify uniforms, not member variables
        void setBool(const std::string &name, bool value) const;
        void setInt(const std::string &name, int value) const;
        void setFloat(const std::string &name, float value) const;

};