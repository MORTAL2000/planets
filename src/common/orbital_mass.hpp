#pragma once

// Standard Headers
#include <vector>
#include <map>
#include <glm/vec3.hpp>


struct OrbitalVelocity {
    float speed;
    float distance;
    float time;
};

class OrbitalMass {
    public:
        OrbitalMass();

        OrbitalMass * center = nullptr;

        float e, i, a, loPE, loAN;
        float rL;
        float SOI;

        double mass;
        float gravParam;
        
        float loANeff = 0, ieff = 0;
        glm::vec3 orbitalAxis;

        bool hasAxialTilt;
        glm::vec3 axialTilt;

        // std::vector<glm::vec3> coords;
        std::map<float, glm::vec3> orbitalPositions;
        std::map<float, float> orbitalTimes;
        std::map<float, OrbitalVelocity> orbitalVelocities;
};