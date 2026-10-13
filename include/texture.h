#pragma once

#include <glad/glad.h>

struct Texture
{
    unsigned int id;
    int width, height, nrChannels;
    unsigned char *data;
    unsigned char *filepath;
};