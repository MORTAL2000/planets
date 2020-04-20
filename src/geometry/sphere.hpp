#pragma once

// Standard Headers
#include <glm/vec3.hpp>

class Sphere {
    public:
        float radius;
        glm::vec3 center;

        Sphere(float radius);

        Sphere(float radius, glm::vec3 center);

        bool rayIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, glm::vec3 *p0, glm::vec3 *p1=NULL) const;
};