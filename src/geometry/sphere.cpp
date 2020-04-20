#include "sphere.hpp"

// Standard Headers
#include <glm/glm.hpp>

Sphere::Sphere(float radius) : radius(radius), center(glm::vec3(0))
{}

Sphere::Sphere(float radius, glm::vec3 center) : radius(radius), center(center)
{}

bool Sphere::rayIntersection(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, glm::vec3 *p0, glm::vec3 *p1) const
{
    // https://www.youtube.com/watch?v=HFPlKQGChpE
    glm::vec3 dir = glm::normalize(rayDirection);
    float t = glm::dot(center - rayOrigin, dir);
    glm::vec3 p = rayOrigin + dir * glm::vec3(t);

    float y = glm::length(center - p);

    if (y > radius) return false;

    float x = sqrt((radius * radius) - (y * y));

    if (p0)
    {
        glm::vec3 point0 = rayOrigin + dir * glm::vec3(t - x);

        p0->x = point0.x;
        p0->y = point0.y;
        p0->z = point0.z;
    }
    if (p1)
    {
        glm::vec3 point1 = rayOrigin + dir * glm::vec3(t + x);

        p1->x = point1.x;
        p1->y = point1.y;
        p1->z = point1.z;
    }
    return true;
}