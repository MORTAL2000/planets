#pragma once

// Standard Headers
#include <glm/vec3.hpp>

// Local Headers
#include "mesh.hpp"
#include "graphics/renderable.hpp"

class TerrainFace {
    private:
        glm::vec3 localUp, axisA, axisB;
        int resolution;
    public:
        SharedMesh mesh;
        TerrainFace(glm::vec3 localUp, int resolution);
        void createMesh();
};