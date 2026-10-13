#pragma once

#include <glad/glad.h>

struct Texture
{
    unsigned int id;
    int width, height, nrChannels;
    unsigned char *data;
    unsigned char *filepath;
};

// class Texture
// {
//     public:
//         unsigned int id; // OpenGL obect ID
//         int width, height, nrChannels; // Textuure metadata
//         unsigned char *data;
//         unsigned char *filepath;

//         Texture(const unsigned char* filepath); // constructor
//         void activate(const GLenum textureUnit);
//         void setTexture

//     private:
//         void genTextureObject();
// };