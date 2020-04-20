#pragma once

#include "common/planet.hpp"

const float EARTH_RADIUS = 150, ATMOSPHERE_RADIUS = 198;

class Universe {
    private:
        double time;
        Planet earth;
    public:
        Universe();
        void update(float dt);
};