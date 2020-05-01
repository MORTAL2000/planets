
#include "planet_generator.hpp"

// Standard Headers
#include <algorithm>

// Local Headers
#include "utils/math_utils.h"
#include "utils/math/interpolation.h"
#include "geometry/cubesphere.hpp"

#include "graphics/tangent_calculator.hpp"


PlanetGenerator::PlanetGenerator() {
    planetNoise.SetNoiseType(FastNoise::Simplex);

    planetNoise.SetFrequency(1);
}

float PlanetGenerator::ridgedNoise(NoiseLayer config, const glm::vec3 & unitSphere, float weightMultiplier) {
        float noiseValue = 0;
        float frequency = 1;
        float amplitude = 1;
        float weight = 1;

        for (int i = 0; i < config.numLayers; i++)
        {
            float v = 1 - std::abs(planetNoise.GetValue(unitSphere.x * frequency, unitSphere.y * frequency, unitSphere.z * frequency));
            v *= v;
            v *= weight;
            weight = std::clamp(v * weightMultiplier, 0.f, 1.f);

            noiseValue += v * amplitude;
            frequency *= config.roughness;
            amplitude *= config.persistence;
        }

        noiseValue = std::max(noiseValue, config.minValue);
        return noiseValue * config.strength;
}

float PlanetGenerator::simpleNoise(NoiseLayer config, const glm::vec3 & unitSphere) {
    float noiseValue = 0;
    float frequency = 1;
    float amplitude = 1;

    for (int i = 0; i < config.numLayers; i++)
    {
        float v = planetNoise.GetValue(unitSphere.x * frequency, unitSphere.y * frequency, unitSphere.z * frequency);
        noiseValue += v * amplitude;
        frequency *= config.roughness;
        amplitude *= config.persistence;
    }

    noiseValue = std::max(noiseValue, config.minValue);
    return noiseValue * config.strength;
}

float PlanetGenerator::calculateElevation(const glm::vec3 & unitSphere) {
    float continent = simpleNoise(CONTINENTS, unitSphere);

    return continent + ridgedNoise(MOUNTAINS, unitSphere, 0.78) * continent;
}

void PlanetGenerator::generate(Planet *plt)
{
    VertAttributes attrs;
    unsigned int posOffset = attrs.add(VertAttributes::POSITION);
    unsigned int norOffset = attrs.add(VertAttributes::NORMAL);
    unsigned int texOffset = attrs.add(VertAttributes::TEX_COORDS);

    PlanetConfig config = plt->config;

    // Make a unit sphere
    Cubesphere sphere(1, config.subdivision, false);

    unsigned int nIndices = sphere.getIndexCount();
    unsigned int nVertices = sphere.getVertexCount();
  
    plt->mesh = std::make_shared<Mesh>(plt->config.name, nVertices, nIndices, attrs);
    
    const float * vertices = sphere.getVertices();
    const float * normals = sphere.getNormals();
    const float * texCords = sphere.getTexCoords();

    

    for (unsigned int i = 0; i < nVertices; i++) {
        glm::vec3 pos(vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2]);
    //        glm::vec3 normal = earth->mesh->get<glm::vec3>(vertI, normOffset);
        glm::vec3 normal = glm::normalize(pos - config.center);

        // Creating noise!
        float height = config.radius + calculateElevation(pos);
        std::cout << "Height for pos: " << glm::to_string(pos) << " " << height << std::endl;
        plt->mesh->set<glm::vec3>(pos + (height * normal), i, posOffset);

        // float elevation = 5 * calculateElevation(pointOnUnitSphere);
    
        // plt->mesh->set(config.radius * ((1 + elevation) * pointOnUnitSphere), i, posOffset);

        plt->mesh->set(glm::vec3(normals[3 * i], normals[3 * i + 1], normals[3 * i + 2]), i, norOffset);
        plt->mesh->set(glm::vec2(texCords[2 * i], texCords[2 * i + 1]), i, texOffset);
    }

    const unsigned int * indices = sphere.getIndices();
    for (unsigned int i = 0; i < nIndices; i++) {
        plt->mesh->indices[i] = indices[i];
    }

    // applyNoiseToMesh(plt->mesh);

    plt->upload();
}
