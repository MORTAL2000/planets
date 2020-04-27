#pragma once

#include "planet.hpp"
#include "utils/generation/planet_generator.hpp"

class Universe {
    private:
        double time;

        PlanetGenerator generator;
        Planet earth;
    public:
        Universe();
        
        double getTime() { return time; }

        void update(float dt);

        glm::vec3 calculateSunDirection();
        Planet * getPlanet();
};
