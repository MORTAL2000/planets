#pragma once

#include "planet.hpp"

class Universe {
    private:
        double time;

        
        Planet earth;
    public:
        Universe(const char *loadFilePath=NULL);
        
        double getTime() { return time; }

        void update(float dt);

        glm::vec3 calculateSunDirection(float lat, float lon, float zoom);
        Planet * getPlanet();
};
