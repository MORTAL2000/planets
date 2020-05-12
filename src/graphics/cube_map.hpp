#pragma once

// System Headers
#include <glad/glad.h>

// Standard Headers
#include <memory>
#include <vector>
#include <string>

class CubeMap;

typedef std::shared_ptr<CubeMap> SharedCubeMap;

class CubeMap
{
  public:
    GLuint id;
    unsigned int width, height;

    unsigned int Texture_Type; // format of texture object
    unsigned int Internal_Format; // format of texture object
    unsigned int Image_Format; // format of texture object

    CubeMap();
    ~CubeMap();

    void generate(unsigned int width, unsigned int height, unsigned char ** buffers);
    void bind(GLuint unit);

};

