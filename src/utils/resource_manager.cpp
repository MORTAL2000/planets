#include "resource_manager.hpp"

// Standard Headers
#include <iostream>
#include <sstream>
#include <fstream>

// System Headers
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

// Local Headers
#include "dds_image.h"


#ifndef ASSETS_DIR
#define ASSETS_DIR ""
#endif

// std::map<string::string, SharedModel> ResourceManager::Models;
std::map<std::string, Shader> ResourceManager::Shaders;
std::map<std::string, SharedTexture> ResourceManager::Textures;

// Created by examining https://github.com/syoyo/tinyobjloader/blob/master/examples/viewer/viewer.cc
static bool FileExists(const char * abs_filename) {
    bool ret;
    FILE* fp = fopen(abs_filename, "rb");
    if (fp) {
        ret = true;
        fclose(fp);
    } else {
        ret = false;
    }

    return ret;
}

Shader ResourceManager::LoadShader(std::string vShaderFile, std::string fShaderFile, std::string name)
{
    Shader shader;
    shader.init_from_files(vShaderFile, fShaderFile);
    Shaders[name] = shader;

    check_gl_error();

    return Shaders[name];
}

SharedTexture ResourceManager::PutTexture(SharedTexture texture, std::string name) {
    Textures[name] = texture;
    return Textures[name];
}

SharedTexture ResourceManager::LoadTexture(std::string file, std::string name)
{
    std::string directory = ASSETS_DIR;

    file = directory + file;

    if(file.substr(file.find_last_of(".") + 1) == "dds") {
        Textures[name] = loadTextureFromDDSFile(file.c_str());
    } else {
        Textures[name] = loadTextureFromFile(file.c_str());
    }
    return Textures[name];
}

SharedTexture ResourceManager::LoadTextureFromMemory(unsigned char *buffer, unsigned int pixels, std::string name)
{
    Textures[name] = loadTextureFromMemory(buffer, pixels);
    return Textures[name];
}


Shader ResourceManager::GetShader(std::string name)
{
    return Shaders[name];
}

SharedTexture ResourceManager::GetTexture(std::string name)
{
    return Textures[name];
}

bool ResourceManager::HasTexture(std::string name) 
{
    std::map<std::string, SharedTexture>::iterator it = Textures.find(name);

    return it != Textures.end();
}

void ResourceManager::Clear()
{
    // (properly) delete all shaders	
    for (auto iter : Shaders)
        glDeleteProgram(iter.second.program_id);
    // (properly) delete all textures
    for (auto iter : Textures)
        glDeleteTextures(1, &iter.second->id);
}

SharedTexture ResourceManager::loadTextureFromDDSFile(const char *file) {
    DDSData data(file);

    Texture * texture = new Texture();
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, data.mipMapCount - 1); // opengl likes array length of mipmaps
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); // don't forget to enable mipmaping
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    unsigned int blockSize = (data.format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0, w = data.width, h = data.height;
    for (unsigned int level = 0; level < data.mipMapCount && (data.width || data.height); ++level)
    {
        unsigned int size = ((w + 3) / 4) * ((h + 3) / 4) * blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, data.format, w, h,
                               0, size, data.buffer + offset);
        offset += size;
        w /= 2;
        h /= 2;
    }

    return SharedTexture(texture);
}

SharedTexture ResourceManager::loadTextureFromFile(const char *file)
{
    // create texture object
    
    if (!FileExists(file)) {
        // Append base dir.
        std::cerr << "Unable to find file: " << file << std::endl;
        exit(1);
    }

    int w, h, nrComponents;
    unsigned char* image = stbi_load(file, &w, &h, &nrComponents, STBI_default);
    
    if (!image) {
        std::cerr << "Unable to load texture: " << file << std::endl;
        exit(1);
    }

    std::cout << "Loaded texture: " << file << ", w = " << w << ", h = " << h << ", nrComponents = " << nrComponents << std::endl;

    Texture * texture = new Texture();

    // Default is GL_RGB, so change the default if it isn't 3 channel
    if (nrComponents == 1) {
        texture->Internal_Format = texture->Image_Format = GL_RED;
    } else if (nrComponents == 4) {
        texture->Internal_Format = texture->Image_Format = GL_RGBA;
    }

    // now generate texture
    texture->generate(w, h, image);
    // and finally free image data
    stbi_image_free(image);

    return SharedTexture(texture);
}

SharedTexture ResourceManager::loadTextureFromMemory(unsigned char *buffer, unsigned int pixels)
{
    int w, h, nrComponents;
    unsigned char *image = stbi_load_from_memory(buffer, pixels, &w, &h, &nrComponents, 0);

    if (!image) {
        std::cerr << "Unable to load embedded texture." << std::endl;
        exit(1);
    }

    std::cout << "Loaded embedded texture w = " << w << ", h = " << h << ", nrComponents = " << nrComponents << std::endl;

    Texture * texture = new Texture();

    // Default is GL_RGB, so change the default if it isn't 3 channel
    if (nrComponents == 1) {
        texture->Internal_Format = texture->Image_Format = GL_RED;
    } else if (nrComponents == 4) {
        texture->Internal_Format = texture->Image_Format = GL_RGBA;
    }

    // now generate texture
    texture->generate(w, h, image);
    // and finally free image data
    stbi_image_free(image);


    return SharedTexture(texture);
}