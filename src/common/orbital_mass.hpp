#pragma once

// Standard Headers
#include <vector>
#include <map>
#include <glm/vec3.hpp>
#include <optional>

struct OrbitalParameters {
    // The ellipse
    float eccentricity = 0; // A circle

    // Orbital Plane
    float inclination = 0; 
    float longitudeAscendingNode = 0;
    float longitudePeriapsis = 0;
    
    // Position 
    float trueAnomaly = 0;
};

struct OrbitalState {
    float speed;
    float distance;
    float time;
    float dt;
    glm::vec3 position;
};

class OrbitalMass {
    public:
        OrbitalMass(OrbitalParameters config, float mass);

        OrbitalMass * center = nullptr;

        void calculateSOI();
        float SOI;

        float e, i, a, loPE, loAN;
        float rL;

        double mass;
        float gravParam;

        float rotEpoch = 0;
        float loANeff = 0, ieff = 0;


        glm::vec3 orbitalAxis;
        std::optional<glm::vec3> axialTilt;

        // std::vector<glm::vec3> coords;
        // std::map<float, glm::vec3> orbitalPositions;
        std::vector<OrbitalState> orbitalPositions;
        // std::map<float, float> orbitalTimes;
        // std::map<float, OrbitalVelocity> orbitalVelocities;
};