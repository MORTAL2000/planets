#pragma once

// Standard Headers
#include <glm/vec3.hpp>
#include "geometry/mesh.hpp"

class Sphere {
    public:
        float radius;
        glm::vec3 center;

        Sphere(float radius);

        Sphere(float radius, glm::vec3 center);

        bool rayIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, glm::vec3 *p0, glm::vec3 *p1=NULL) const;

        /**
         * Generates a Sphere mesh.
         * Define what attributes the vertices should have in 'attrs'.
         * POSITION is required, NORMAL, TANGENT and TEX_COORDS are supported.
         **/
        SharedMesh generate(const std::string &meshName, int segments, int rings, VertAttributes &attrs);
};