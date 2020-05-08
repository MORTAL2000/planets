#include "texture.hpp"

// Standard Headers
#include <iostream>

// Local Headers
#include "graphics/gl_error.hpp"


Texture::Texture()
    : width(0), 
    height(0),
    Internal_Format(GL_RGB),
    Image_Format(GL_RGB),
    Texture_Type(GL_UNSIGNED_BYTE),
    Max_Level(1000),
    Wrap_S(GL_REPEAT), 
    Wrap_T(GL_REPEAT), 
    Filter_Min(GL_LINEAR_MIPMAP_LINEAR), 
    Filter_Max(GL_LINEAR)
{
    glGenTextures(1, &this->id);
    std::cout << "Texture id: " << id << " created\n";
}

void Texture::generate(unsigned int width, unsigned int height, unsigned char* data)
{
    this->width = width;
    this->height = height;

    check_gl_error();

    // create Texture
    glBindTexture(GL_TEXTURE_2D, this->id);
    
    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0); // opengl likes array length of mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, Max_Level); // opengl likes array length of mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);

    // create Texture
    glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, this->Texture_Type, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    check_gl_error();
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, data.mipMapCount - 1); // opengl likes array length of mipmaps

    // unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture::generateCompressed(unsigned int width, unsigned int height, unsigned int blockSize, unsigned char* data) {
    this->width = width;
    this->height = height;

    check_gl_error();

    // create Texture
    glBindTexture(GL_TEXTURE_2D, this->id);

    // set Texture wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0); // opengl likes array length of mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, Max_Level); // opengl likes array length of mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
    
    unsigned int offset = 0, w = width, h = height;
    for (unsigned int level = 0; level <= Max_Level && (width || height); ++level)
    {
        unsigned int size = ((w + 3) / 4) * ((h + 3) / 4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, this->Internal_Format, w, h, 0, size, data + offset);
        offset += size;
        w /= 2;
        h /= 2;
    }
}

void Texture::bind(GLuint unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D, id);
}

Texture::~Texture()
{
    glDeleteTextures(1, &id);
    std::cout << "Texture " << id << " destroyed\n";
}
