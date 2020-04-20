#pragma once

// Standard Headers
#include <iostream>
#include <vector>
#include <glm/vec2.hpp>

// Local Headers
#include "geometry/sphere.hpp"
#include "rendering/camera.hpp"
class Planet {
    public:
        Planet(float radius);
        Sphere sphere;

        float longitude(float x, float z) const;
        float latitude(float y) const;
        static glm::vec2 deltaLonLat(glm::vec2 a, glm::vec2 b);
        bool rayToLonLat(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, glm::vec2 &lonLat) const;
};