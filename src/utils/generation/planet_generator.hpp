#pragma once
#include "common/planet.hpp"

// System Headers
// #include <FastNoiseSIMD/FastNoiseSIMD.h>
#include "utils/math/FastNoise.h"

struct NoiseLayer {
    int numLayers;

    float strength = 1;
    float baseRoughness = 1.f;
    float roughness = 2.f;
    float persistence = 0.5f;
    float minValue;
};

static NoiseLayer CONTINENTS = {
    4, 2.5f, 1.07, 2.2, 0.5, -4
};

static NoiseLayer MOUNTAINS = {
    4, 5.42, 2.59, 4.3, 0.8, 0.37
};

class PlanetGenerator {
private:
    FastNoise planetNoise;

    float ridgedNoise(NoiseLayer config, const glm::vec3 & unitSphere, float weightMultiplier);
    float simpleNoise(NoiseLayer config, const glm::vec3 & unitSphere);
    float calculateElevation(const glm::vec3 & unitSphere);
public:
    PlanetGenerator();
    void generate(Planet *plt);
};
