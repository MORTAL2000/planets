#include "planet.hpp"

// Standard Headers
#include <math.h>
#include <vector>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/intersect.hpp>

// Local Headers
#include "graphics/vert_buffer.hpp"
#include "utils/math_utils.h"
#include "utils/orbital_utils.h"
#include "utils/serialization.h"


Planet::Planet(PlanetConfig config): name(config.name), config(config), OrbitalMass(config.orbit, config.mass) {
    std::cout << "Planet " << name << " created\n";
} 

void Planet::upload()
{
    VertBuffer::uploadSingleMesh(terrainMesh);
    VertBuffer::uploadSingleMesh(waterMesh);
    VertBuffer::uploadSingleMesh(atmosphereMesh);
}

void Planet::uploadOrbit() {
    // Create the shared mesh
    if (!center) return;

    int nVerticies = orbitalPositions.size();

    VertAttributes orbitAttrs;
    unsigned int posOffset = orbitAttrs.add(VertAttributes::POSITION);
    orbitMesh = std::make_shared<Mesh>(config.name + "_orbit_mesh", nVerticies, nVerticies, orbitAttrs);
    orbitMesh->mode = GL_LINE_LOOP;

    // Fixed to the center right now
    // float radius = glm::length(get_position());

    for (int i = 0; i < nVerticies; i++) {
        // Circle
        // float degree = i / ou::ORBIT_RESOLUTION;
        // float x = radius * cos(glm::radians(degree));
        // float y = radius * sin(glm::radians(degree));
        // orbitMesh->set<glm::vec3>(glm::vec3(x, 0, y) - _origin, i, posOffset);

        // Orbital Math
        orbitMesh->set<glm::vec3>(orbitalPositions[i].position, i, posOffset);
        std::cout << "Orbital Degree: " << i << glm::to_string(orbitalPositions[i].position) << std::endl;
        std::cout << " at time: " << i << orbitalPositions[i].time << std::endl;

        orbitMesh->indices[i] = i;
    }

    VertBuffer::uploadSingleMesh(orbitMesh);
}

void Planet::update(float time) {
    set_position(ou::findPlanetLocation(this, time));
}

void Planet::render(RenderType type)
{
    if (type == RenderType::Terrain) {
        terrainMesh->render();
    }

    if (type == RenderType::Water) {
        waterMesh->render();
    }

    if (type == RenderType::Path) {
        if (center) orbitMesh->render();
    }

    if (type == RenderType::Atmosphere) {
        atmosphereMesh->render();
    }
}

float Planet::longitude(float x, float z) const
{
    return mu::RAD_TO_DEGREES * std::atan2(z, x) + 180.0f;
}

float Planet::latitude(float y) const
{
    return mu::RAD_TO_DEGREES * glm::acos((y) / config.radius);
}

float minAbs(float a, float b)
{
    return abs(a) < abs(b) ? a : b;
}

vec2 Planet::deltaLonLat(vec2 a, vec2 b)
{
    glm::vec2 minP = a.x < b.x ? a : b;
    glm::vec2 maxP = a.x < b.x ? b : a;

    return vec2(
        minAbs(b.x - a.x, ((maxP.x - 360) - minP.x) * (a.x < b.x ? 1 : -1)),

        b.y - a.y
    );
}

bool Planet::rayToLonLat(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, glm::vec2 &lonLat)
{
    glm::vec3 intersection, normal;
    const glm::vec3 & position = this->get_position();
    if (glm::intersectRaySphere(rayOrigin, rayDirection, position, config.radius, intersection, normal))
    {
        intersection -= position;
        lonLat.x = longitude(intersection.x, intersection.z);
        lonLat.y = latitude(intersection.y);
        return true;
    }
    else return false;
}

glm::vec3 Planet::lonLatTo3d(float lon, float lat, float altitude) const
{
    glm::vec3 out(0, config.radius + altitude, 0);
    out = glm::rotate(out, lat * mu::DEGREES_TO_RAD, mu::Z);
    return glm::rotate(out, -lon * mu::DEGREES_TO_RAD, mu::Y);
}

glm::vec3 Planet::calculatePointOnPlanet(glm::vec3 pointOnUnitSphere) {
    return pointOnUnitSphere * config.radius;
}
