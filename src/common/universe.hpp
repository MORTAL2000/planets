#pragma once

#include "orbital_mass.hpp"
#include "sun.hpp"
#include "planet.hpp"
#include "utils/generation/planet_generator.hpp"

class Universe {
    private:
        double time;
        float simulationSpeed = 1.f;

        PlanetGenerator generator;

        std::vector<Sun *> suns;
        std::vector<Planet *> planets;
        std::vector<Renderable *> renderables;
        OrbitalMass * center;

        bool debugOpen = false;

        void generateOrbitalData();
    public:
        Universe();
        
        const double & getTime() const { return time; }
        const float & getSpeed() const { return simulationSpeed; }

        void update(float dt);

        const std::vector<Planet*> & getPlanets() const;
        const std::vector<Renderable*> & getRenderables() const;
        const std::vector<Sun*> & getSuns() const;
};
