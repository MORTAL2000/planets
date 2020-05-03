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
    4, 3.08f, 1.07, 1.5, 1.1, -1.2
};

static NoiseLayer MOUNTAINS = {
    3, 0.376, 9.662, 8.120, 4.436, 0.37
};

// static NoiseLayer MOUNTAINS = {
//     2, 1.391, 9.662, 8.120, 4.5, -2.867
// };

class PlanetGenerator {
private:
    FastNoise planetNoise;
    float ridgedNoise(NoiseLayer config, const glm::vec3 & unitSphere, float weightMultiplier);
    float simpleNoise(NoiseLayer config, const glm::vec3 & unitSphere);
    float calculateElevation(const glm::vec3 & unitSphere);
public:
    PlanetGenerator();
    void generate(Planet *plt);
    static void ShowDebugWindow(bool* p_open);
};
