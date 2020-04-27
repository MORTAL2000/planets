#pragma once

#include "common/planet.hpp"

const float EARTH_RADIUS = 150, ATMOSPHERE_RADIUS = 198;

class PlanetGenerator {
    public:
        PlanetGenerator();
        PlanetConfig generate();
};