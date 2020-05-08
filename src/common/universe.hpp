#pragma once

#include "planet.hpp"
#include "utils/generation/planet_generator.hpp"

class Universe {
    private:
        double time;

        PlanetGenerator generator;
        Planet earth;
        bool debugOpen = false;
    public:
        Universe();
        
        double getTime() { return time; }

        void update(float dt);

        glm::vec3 calculateSunDirection(float lat, float lon, float zoom);
        Planet * getPlanet();
};
