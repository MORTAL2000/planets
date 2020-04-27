#pragma once

// Standard Headers
#include <iostream>
#include <vector>
#include <glm/vec2.hpp>

// Local Headers
#include "geometry/sphere.hpp"
#include "geometry/terrain_face.hpp"
#include "graphics/renderable.hpp"
#include "graphics/camera.hpp"

struct PlanetConfig {
    std::string name;
    float radius;
    int segments;
    int rings;
};

class Planet {
    private:
        // TerrainFace * faces[6];
    public:
        std::string name;
        Sphere sphere;
        SharedMesh mesh;

        Planet(PlanetConfig config);
        PlanetConfig config;

        void upload();
        void render();
        glm::vec3 calculatePointOnPlanet(glm::vec3 pointOnUnitSphere);

        // Planet Math
        float longitude(float x, float z) const;
        float latitude(float y) const;
        static glm::vec2 deltaLonLat(glm::vec2 a, glm::vec2 b);
        glm::vec3 lonLatTo3d(float lon, float lat, float altitude) const;

        bool rayToLonLat(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, glm::vec2 &lonLat) const;
        bool cursorToLonLat(const Camera *cam, glm::vec2 &lonLat) const;

        void uploadMeshes();
};