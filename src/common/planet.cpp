#include "planet.hpp"

#include <math.h>
#include "glm/gtx/rotate_vector.hpp"

const float RAD_TO_DEGREES = 180.0f / M_PI;

Planet::Planet(float radius): sphere(radius) {

} 

float Planet::longitude(float x, float z) const
{
    return RAD_TO_DEGREES * std::atan2(z, x) + 180.0f;
}

float Planet::latitude(float y) const
{
    return RAD_TO_DEGREES * glm::acos((y) / sphere.radius);
}

float minAbs(float a, float b)
{
    return abs(a) < abs(b) ? a : b;
}

glm::vec2 Planet::deltaLonLat(glm::vec2 a, glm::vec2 b)
{
    glm::vec2 minP = a.x < b.x ? a : b;
    glm::vec2 maxP = a.x < b.x ? b : a;

    return glm::vec2(
        minAbs(b.x - a.x, ((maxP.x - 360) - minP.x) * (a.x < b.x ? 1 : -1)),

        b.y - a.y
    );
}

bool Planet::rayToLonLat(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, glm::vec2 &lonLat) const
{
    glm::vec3 intersection;
    if (sphere.rayIntersection(rayOrigin, rayDirection, &intersection, NULL))
    {
        lonLat.x = longitude(intersection.x, intersection.z);
        lonLat.y = latitude(intersection.y);
        return true;
    }
    else return false;
}