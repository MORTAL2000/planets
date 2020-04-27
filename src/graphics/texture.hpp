#pragma once

// System Headers
#include <glad/glad.h>
#include <memory>

// Local Headers
// #include "graphics/shader.hpp"

class Texture;

typedef std::shared_ptr<Texture> SharedTexture;

class Texture 
{
    public:
        GLuint id;
        unsigned int width, height;

        // texture Format
        unsigned int Internal_Format; // format of texture object
        unsigned int Image_Format; // format of loaded image
        unsigned int Texture_Type; // type of texture data image
        // texture configuration
        unsigned int Wrap_S; // wrapping mode on S axis
        unsigned int Wrap_T; // wrapping mode on T axis
        unsigned int Filter_Min; // filtering mode if texture pixels < screen pixels
        unsigned int Filter_Max; // filtering mode if texture pixels > screen pixels

        Texture();

        void generate(unsigned int width, unsigned int height, unsigned char* data);
        void bind(GLuint unit);
};