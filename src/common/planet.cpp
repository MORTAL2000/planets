#include "planet.hpp"

// Standard Headers
#include <math.h>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/intersect.hpp>

// Local Headers
#include "graphics/vert_buffer.hpp"
#include "utils/math_utils.h"
#include "utils/serialization.h"


Planet::Planet(PlanetConfig config): name(config.name), config(config) {
    std::cout << "Planet " << name << " created\n";
} 

void Planet::upload()
{
    // VertBuffer *buffer = NULL;
    // for (auto isl : land)
    // {
    //     if (!buffer) buffer = VertBuffer::with(isl->terrainMesh->attributes);
    //     buffer->add(isl->terrainMesh);
    // }
    // if (buffer) buffer->upload();
    VertBuffer::uploadSingleMesh(terrainMesh);
    VertBuffer::uploadSingleMesh(waterMesh);
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

bool Planet::rayToLonLat(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection, glm::vec2 &lonLat) const
{
    glm::vec3 intersection, normal;
    if (glm::intersectRaySphere(rayOrigin, rayDirection, glm::vec3(0.f), config.radius, intersection, normal))
    {
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

// void Planet::createMesh() {
//     Cubesphere sphere(config.radius, 3, false);

//     VertAttributes attrs;
//     attrs.add_(VertAttributes::POSITION)
//         .add_(VertAttributes::NORMAL)
//         .add_(VertAttributes::TANGENT)
//         .add_(VertAttributes::TEX_COORDS);
// }

void Planet::toBinary(std::vector<uint8> &out) const
{
    // slz::add((uint8) land.size(), out);
    // for (int i = 0; i < land.size(); i++)
    //     slz::add(uint32(0), out);

    // for (int i = 0; i < land.size(); i++)
    // {
    //     uint32 beginSize = out.size();
    //     land[i]->toBinary(out);
    //     uint32 size = out.size() - beginSize;

    //     memcpy(&out[1 + i * 4], &size, 4);
    // }
}

void Planet::fromBinary(const std::vector<uint8> &in, unsigned int inputOffset)
{
    // uint8 nrOfIslands = in.at(inputOffset);

    // uint32 startI = nrOfIslands * 4 + 1 + inputOffset;
    // for (int i = 0; i < nrOfIslands; i++)
    // {
    //     auto size = slz::get<uint32>(in, inputOffset + 1 + i * 4);

    //     land.emplace_back(new LandMass(in, startI, this));
    //     auto isl = land.back();
    //     isl->planetDeform();
    //     isl->placeOnPlanet();
    //     isl->transformVertices();
    //     isl->calculateNormals();
    //     isl->createMesh();
    //     startI += size;
    // }
    
    VertAttributes attrs;
    attrs.add_(VertAttributes::POSITION)
        .add_(VertAttributes::NORMAL)
        .add_(VertAttributes::TANGENT)
        .add_(VertAttributes::TEX_COORDS);

    // mesh = sphere.generate(name + "_mesh", config.segments, config.rings, attrs);
//    createMesh();
    upload();
    std::cout << "planet loaded from binary\n";
}

