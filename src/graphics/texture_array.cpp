#include "texture_array.hpp"

// Standard Headers
#include <iostream>

// Local Headers
#include "graphics/gl_error.hpp"

// SharedTexArray TextureArray::fromByteData(const GLubyte *data, GLenum format, GLsizei width, GLsizei height, GLsizei depth)
// {
//     GLuint id;
//     glGenTextures(1, &id);
//     glBindTexture(GL_TEXTURE_2D_ARRAY, id);

//     glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, format, width, height, depth, 0, format, GL_UNSIGNED_BYTE, data);

//     glGenerateMipmap(GL_TEXTURE_2D_ARRAY);

//     glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//     glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // don't forget to enable mipmaping
//     glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
//     glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

//     return SharedTexArray(new TextureArray(id, width, height, depth));
// }


TextureArray::TextureArray(): 
    width(0), 
    height(0), 
    layers(0), 
    Max_Level(1000),
    Image_Format(GL_RGB),
    Internal_Format(GL_RGB), 
    Texture_Type(GL_UNSIGNED_BYTE)
{
    glGenTextures(1, &this->id);
    std::cout << "TextureArray id: " << id << " created\n";
}

void TextureArray::generate(unsigned int width, unsigned int height, unsigned int layers, unsigned char ** buffers)
{
    this->width = width;
    this->height = height;
    this->layers = layers;

    std::cout << "Generating TextureArray width: " << width << " height: " << height << " layers: " << layers << "\n";

    check_gl_error();

    // create Texture
    glBindTexture(GL_TEXTURE_2D_ARRAY, this->id);

    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAX_LEVEL, Max_Level); // opengl likes array length of mipmaps
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // don't forget to enable mipmaping
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    GLuint blockSize = (Internal_Format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;

    GLuint offset = 0, w = width, h = height;
    for (unsigned int mipMapLevel = 0; mipMapLevel <= Max_Level; mipMapLevel++)
    {
        GLuint size = ((w + 3) / 4) * ((h + 3) / 4) * blockSize;
        glCompressedTexImage3D(GL_TEXTURE_2D_ARRAY, mipMapLevel, Internal_Format, w, h, layers, 0, size * layers, 0);
        
        for (unsigned int i = 0; i < this->layers; i++) {
            std::cout << "Buffer generated: " << buffers[i] << std::endl;
            glCompressedTexSubImage3D(GL_TEXTURE_2D_ARRAY, mipMapLevel, 0, 0, i, w, h, 1, Internal_Format, size, buffers[i] + offset);
        }

        offset += size;
        w /= 2;
        h /= 2;
    }

    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    // glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, data.mipMapCount - 1); // opengl likes array length of mipmaps

    // unbind texture
    glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void TextureArray::bind(GLuint unit)
{
    glActiveTexture(GL_TEXTURE0 + unit);
    glBindTexture(GL_TEXTURE_2D_ARRAY, id);
}

TextureArray::~TextureArray()
{
    glDeleteTextures(1, &id);
    std::cout << "TextureArray " << id << " destroyed\n";
}
