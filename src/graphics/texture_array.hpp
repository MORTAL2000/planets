#pragma once

// System Headers
#include <glad/glad.h>

// Standard Headers
#include <memory>
#include <vector>
#include <string>

class TextureArray;

typedef std::shared_ptr<TextureArray> SharedTexArray;

class TextureArray
{
  public:
    GLuint id;
    unsigned int width, height, layers;

    unsigned int Texture_Type; // format of texture object
    unsigned int Image_Format; // format of texture object
    unsigned int Internal_Format; // format of texture object
    unsigned int Max_Level; // Highest defined mipmap level

    TextureArray();
    ~TextureArray();

    void generate(unsigned int width, unsigned int height, unsigned int layers, unsigned char* data);
    void bind(GLuint unit);
};
