#pragma once

// Standard Headers
#include <iostream>
#include <vector>
#include <glm/vec2.hpp>

// Local Headers
#include "orbital_mass.hpp"
#include "geometry/sphere.hpp"
#include "graphics/renderable.hpp"
#include "graphics/camera.hpp"

struct PlanetConfig {
    std::string name;

    float radius;
    // glm::vec3 center = glm::vec3(0.f);
    
    // Mesh
    int subdivision = 5; 

    // Noise
    float roughness = 1;
};

class Planet: public OrbitalMass, public Renderable {
    private:
        // TerrainFace * faces[6];
    public:
        std::string name;
        // Sphere sphere;

        
        SharedMesh terrainMesh;
        SharedMesh waterMesh;

        /**
         * Texture map of the planet.
         * An planet can have 5 textures.
         * 1 background texture and 4 extra textures.
         *
         * Each vertex has a vec4.
         * 
         * The n-th float of that vec4 tells how much of n-th texture should be rendered at the position of that vertex.
         */
        // std::vector<vec4> textureMap;

        Planet(PlanetConfig config);
        PlanetConfig config;

        void generateOrbitalCoords();
        void generateOrbitalTimes();

        void upload();
        void render(RenderType type);

        glm::vec3 calculatePointOnPlanet(glm::vec3 pointOnUnitSphere);

        // Planet Math
        float longitude(float x, float z) const;
        float latitude(float y) const;
        static glm::vec2 deltaLonLat(glm::vec2 a, glm::vec2 b);
        glm::vec3 lonLatTo3d(float lon, float lat, float altitude) const;

        bool rayToLonLat(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, glm::vec2 &lonLat) const;
        
        void toBinary(std::vector<uint8> &out) const;

        void fromBinary(const std::vector<uint8> &in, unsigned int inputOffset = 0);
};
