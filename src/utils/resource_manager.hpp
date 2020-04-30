#pragma once

// Inspired by https://learnopengl.com/code_viewer.php?code=in-practice/breakout/resource_manager.h

// Standard Headers
#include <map>
#include <string>

// Local Headers
#include "graphics/shader.hpp"
#include "graphics/texture.hpp"
#include "graphics/texture_array.hpp"
#include "graphics/cube_map.hpp"
#include "geometry/model.hpp"

class ResourceManager 
{
    public:
        // static std::map<std::string, SharedModel>   Models;
        static std::map<std::string, Shader>              Shaders;
        static std::map<std::string, SharedTexture>       Textures;
        static std::map<std::string, SharedTexArray>      TextureArrays;
        static std::map<std::string, SharedCubeMap>       CubeMaps;
        static std::map<std::string, SharedModel>         Models;

        // Loads a model
        // static SharedModel   LoadModel(std::string file, std::string name);
        // static SharedModel   GetModel(std::string name);

        // Loads (and generates) a shader program from file loading vertex, fragment (and geometry) shader's source code. If gShaderFile is not nullptr, it also loads a geometry shader
        static Shader           LoadShader(std::string vShaderFile, std::string fShaderFile, std::string name);
        static Shader           GetShader(std::string name);

        static SharedTexture    PutTexture(SharedTexture texture, std::string name);
        static SharedTexture    LoadTexture(std::string file, std::string name);
        static SharedTexture    LoadTextureFromMemory(unsigned char *buffer, unsigned int pixels, std::string name);
        static SharedTexture    GetTexture(std::string name);
        static bool             HasTexture(std::string name);

        static SharedTexArray   LoadTextureArray(const std::vector<std::string> &paths, std::string name);
        static SharedTexArray   GetTextureArray(std::string name);

        static SharedCubeMap    LoadCubeMap(const std::vector<std::string> &paths, std::string name);
        static SharedCubeMap    GetCubeMap(std::string name);
        
        static SharedModel      LoadModel(std::string file, std::string name);
        static SharedModel      GetModel(std::string name);

        // Properly de-allocates all loaded resources
        static void     Clear();

    private:
        // Private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
        ResourceManager() { }

        static SharedTexture loadTextureFromFile(const char * file);
        static SharedTexture loadTextureFromDDSFile(const char * file);
        static SharedTexture loadTextureFromMemory(unsigned char *buffer, unsigned int pixels);
        static SharedTexArray loadTextureArrayFromDDSFiles(const std::vector<std::string> &paths);
        static SharedCubeMap loadCubeMapFromDDSFiles(const std::vector<std::string> &paths);
};
