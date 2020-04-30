
#include "cube_map.hpp"

// Standard Header
#include <iostream>

// Local Headers
#include "graphics/gl_error.hpp"

CubeMap::CubeMap()
    : width(0), height(0)
{
    glGenTextures(1, &this->id);
    std::cout << "CubeMap id: " << id << " created\n";
}

void CubeMap::bind(GLuint unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_CUBE_MAP, id);
}

void CubeMap::generate(unsigned int width, unsigned int height, std::vector<unsigned char*> buffers)
{
    this->width = width;
    this->height = height;

    check_gl_error();

    // create Texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, this->id);

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    check_gl_error();

    GLuint blockSize = (Internal_Format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    GLuint size = ((width + 3) / 4) * ((height + 3) / 4) * blockSize;

    int i = 0;
    for (auto &buffer : buffers)
    {
        glCompressedTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, Internal_Format, width, height, 0, size, buffer);
        i++;
    }
    
    // unbind texture
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

CubeMap::~CubeMap()
{
    glDeleteTextures(1, &id);
    std::cout << "CubeMap " << id << " destroyed\n";
}
