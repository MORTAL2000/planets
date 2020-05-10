#pragma once

#include "orbital_mass.hpp"
#include "planet.hpp"
#include "utils/generation/planet_generator.hpp"

class Universe {
    private:
        double time;

        PlanetGenerator generator;

        std::vector<OrbitalMass *> planets;
        OrbitalMass * center;

        bool debugOpen = false;

        void generateOrbitalData();
    public:
        Universe();
        
        double getTime() { return time; }

        void update(float dt);

        glm::vec3 calculateSunDirection(float lat, float lon, float zoom);
        Planet * getPlanet();
};
