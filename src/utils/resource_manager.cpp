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
std::map<std::string, SharedTexArray> ResourceManager::TextureArrays;
std::map<std::string, SharedCubeMap> ResourceManager::CubeMaps;
std::map<std::string, SharedModel> ResourceManager::Models;

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

SharedTexArray  ResourceManager::LoadTextureArray(const std::vector<std::string> &paths, std::string name) {
    if (paths.size() == 0)
        throw "TextureArray::fromDDSFiles needs at least 1 DDS file";

    TextureArrays[name] = loadTextureArrayFromDDSFiles(paths);
    
    return TextureArrays[name];
}

SharedCubeMap  ResourceManager::LoadCubeMap(const std::vector<std::string> &paths, std::string name) {
    if (paths.size() != 6)
        throw "CubeMap::fromDDSFiles needs 6 DDS files";

    CubeMaps[name] = loadCubeMapFromDDSFiles(paths);
    
    return CubeMaps[name];
}

SharedCubeMap  ResourceManager::GetCubeMap(std::string name) {
    return CubeMaps[name];
}

SharedTexArray  ResourceManager::GetTextureArray(std::string name) {
    return TextureArrays[name];
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

SharedModel ResourceManager::LoadModel(std::string file, std::string name)
{
    std::string directory = ASSETS_DIR;

    file = directory + file;
    Models[name] = SharedModel(new Model(file, glm::vec3(1.f), false)); 

    return Models[name];
}

SharedModel ResourceManager::GetModel(std::string name)
{
    return Models[name];
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


// Private

SharedTexArray ResourceManager::loadTextureArrayFromDDSFiles(const std::vector<std::string> &paths) {
    std::string directory = ASSETS_DIR;

    std::vector<DDSData> datas;
    datas.reserve(paths.size());
    for (auto &path : paths)
        datas.emplace_back((ASSETS_DIR + path).c_str());

    TextureArray * texarray = new TextureArray();
    DDSData &d = datas[0];

    GLuint width = d.width, height = d.height, layerCount = datas.size(), mipMapCount = d.mipMapCount;

    texarray->Max_Level = mipMapCount;
    texarray->Internal_Format = d.format;

    unsigned char ** buffers = new unsigned char *[layerCount];
    for (unsigned int i = 0; i < layerCount; i++) {
        if (datas[i].width != width || datas[i].height != height || datas[i].format != d.format || datas[i].mipMapCount != mipMapCount)
            throw "TextureArray: DDS files cannot have different dimensions/formats/mipmap-levels";

        buffers[i] = datas[i].buffer;
    }

    texarray->generate(width, height, layerCount, buffers);
    
    delete [] buffers;

    return SharedTexArray(texarray);
}


SharedCubeMap ResourceManager::loadCubeMapFromDDSFiles(const std::vector<std::string> &paths) {
    std::string directory = ASSETS_DIR;

    std::vector<DDSData> datas;
    datas.reserve(6);
    for (auto &path : paths)
        datas.emplace_back((ASSETS_DIR + path).c_str());

    CubeMap * cube_map = new CubeMap();
    DDSData &d = datas[0];

    GLuint width = d.width, height = d.height;

    cube_map->Internal_Format = d.format;

    std::vector<unsigned char *> buffers;
    for (auto &data : datas)
    {
        if (data.width != width || data.height != height || data.format != d.format)
            throw "CubeMap: DDS files cannot have different dimensions/formats/mipmap-levels";

        buffers.push_back(data.buffer);
    }

    cube_map->generate(width, height, buffers);

    return SharedCubeMap(cube_map);
}

SharedTexture ResourceManager::loadTextureFromDDSFile(const char *file) {
    DDSData data(file);

    Texture * texture = new Texture();

    texture->Max_Level = data.mipMapCount - 1;
    texture->Filter_Min = GL_LINEAR_MIPMAP_LINEAR;
    texture->Filter_Max = GL_LINEAR;
    texture->Wrap_S = GL_REPEAT;
    texture->Wrap_T = GL_REPEAT;
    texture->Internal_Format = data.format;

    unsigned int blockSize = (data.format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    texture->generateCompressed(data.width, data.height, blockSize, data.buffer);

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

