#pragma once
#include "common/planet.hpp"

// System Headers
// #include <FastNoiseSIMD/FastNoiseSIMD.h>
#include "utils/math/FastNoise.h"

struct VertexCharacteristics {
    float height;
    float minNeighbor;
    float maxNeighbor;
    float distToGrass;
    float distToRock;
    // float nearestRock;
    // float nearestSea;
};

struct NoiseLayer {
    int numLayers;

    float strength = 1;
    float baseRoughness = 1.f;
    float roughness = 2.f;
    float persistence = 0.5f;
    float minValue;
};

namespace {
    static float SEA_BOTTOM = -5, GRASS_LEVEL = 0.5, LAND_LEVEL = 1.0f, ROCK_LEVEL = 9.f, SNOW_LEVEL = 18.f;
    const int ROCK_TEX = 0, GRASS_TEX = 1, DEAD_GRASS_TEX = 2, ROCK2_TEX = 3;

    static float DEAD_GRASS_NOISE_OFFSET = 500;

    static NoiseLayer CONTINENTS = {
    4, 3.08f, 1.07, 1.5, 1.1, -1.2
    };

    static NoiseLayer MOUNTAINS = {
        3, 0.376, 9.662, 8.120, 4.436, 0.37
    };
}

class PlanetGenerator {
private:
    
    FastNoise planetNoise;
    FastNoise grassNoise;
    FastNoise deadGrassNoise;
    float ridgedNoise(NoiseLayer config, const glm::vec3 & unitSphere, float weightMultiplier);
    float simpleNoise(NoiseLayer config, const glm::vec3 & unitSphere);
    float calculateElevation(const glm::vec3 & unitSphere);

    float distToHeight(const glm::vec3 & unitSphere, float minHeight, float maxHeight, int maxDist) const;

    float calculateGrass(const glm::vec3 & unitSphere, VertexCharacteristics v);
    float calculateDeadGrass(const glm::vec3 & unitSphere, VertexCharacteristics v);
    float calculateRock(const glm::vec3 & unitSphere, VertexCharacteristics v);
    float calculateRock2(const glm::vec3 & unitSphere, VertexCharacteristics v);

    void addTextureMaps(Mesh * mesh);
public:
    PlanetGenerator();
    void generate(Planet *plt);
    static void ShowDebugWindow(bool* p_open);
};
