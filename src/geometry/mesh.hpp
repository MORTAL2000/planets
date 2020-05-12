#pragma once

// System Headers
#include <glad/glad.h>

// Standard Headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <memory>
#include <vector>

// Local Headers
#include "graphics/renderable.hpp"
#include "graphics/vert_data.hpp"

struct Material {
    float ambient[3];
    float diffuse[3];
    float specular[3];
    float emission[3];
    float shininess;

    Material() {
        specular[0] = specular[1] = specular[2] = 0.0f;
        shininess = 0.0f;
    }
};

class VertBuffer;
class Mesh;

typedef std::shared_ptr<Mesh> SharedMesh;

class Mesh : public VertData {
    public:
        static SharedMesh getQuad();

        std::string name;
        std::vector<unsigned short> indices;
        GLenum mode = GL_TRIANGLES;

        unsigned int nrOfVertices, nrOfIndices;
        int baseVertex = 0, indicesBufferOffset = 0;

        VertBuffer *vertBuffer = nullptr;

        Mesh(const std::string& name, unsigned int nrOfVertices, unsigned int nrOfIndices, VertAttributes attributes);
        ~Mesh();

        Material material;
        std::vector<std::string> textures;

        void computeSmoothingNormals();

        void render();
        void renderInstances(GLsizei count);
    private:
        static SharedMesh quad;
};