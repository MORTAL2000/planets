
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
    VertAttributes terrainAttrs;
    unsigned int posOffset = terrainAttrs.add(VertAttributes::POSITION);
    unsigned int norOffset = terrainAttrs.add(VertAttributes::NORMAL);
    unsigned int texOffset = terrainAttrs.add(VertAttributes::TEX_COORDS);
    unsigned int tanOffset = terrainAttrs.add(VertAttributes::TANGENT);
    unsigned int yLevelOffset = terrainAttrs.add({"Y_LEVEL", 1});

    VertAttributes waterAttrs;
    waterAttrs.add_(VertAttributes::POSITION)
        .add_(VertAttributes::NORMAL)
        .add_(VertAttributes::TEX_COORDS)
        .add_(VertAttributes::TANGENT);

    PlanetConfig config = plt->config;

    // Make a unit sphere
    Cubesphere cubesphere(1, config.subdivision, false);

    Sphere sphere(config.radius);

    unsigned int nIndices = cubesphere.getIndexCount();
    unsigned int nVertices = cubesphere.getVertexCount();
  
    plt->terrainMesh = std::make_shared<Mesh>(plt->config.name + "_terrain", nVertices, nIndices, terrainAttrs);
    plt->waterMesh = sphere.generate(plt->config.name + "_water", 100, 70, waterAttrs); //sphere.gstd::make_shared<Mesh>(plt->config.name + "_water", nVertices, nIndices, attrs);
    
    const float * vertices = cubesphere.getVertices();
    const float * normals = cubesphere.getNormals();
    const float * texCords = cubesphere.getTexCoords();


    for (unsigned int i = 0; i < nVertices; i++) {
        // plt->waterMesh->set(glm::vec3(vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2]), i, posOffset);
        

        glm::vec3 pos(vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2]);
    //        glm::vec3 normal = earth->mesh->get<glm::vec3>(vertI, normOffset);
        glm::vec3 normal = glm::normalize(pos - config.center);

        // Creating noise!
        float terrainHeight = calculateElevation(pos);

        plt->terrainMesh->set(terrainHeight, i, yLevelOffset);

        std::cout << "Height for pos: " << glm::to_string(pos) << " " << terrainHeight  + config.radius << std::endl;
        plt->terrainMesh->set<glm::vec3>(pos + ((terrainHeight + config.radius) * normal), i, posOffset);

        // float elevation = 5 * calculateElevation(pointOnUnitSphere);
    
        // plt->mesh->set(config.radius * ((1 + elevation) * pointOnUnitSphere), i, posOffset);
        // Recalculate textures and normals
        plt->terrainMesh->set(glm::vec3(normals[3 * i], normals[3 * i + 1], normals[3 * i + 2]), i, norOffset);
        plt->terrainMesh->set(glm::vec2(texCords[2 * i], texCords[2 * i + 1]), i, texOffset);
    }

    const unsigned int * indices = cubesphere.getIndices();
    for (unsigned int i = 0; i < nIndices; i++) {
        plt->terrainMesh->indices[i] = indices[i];
    }

    TangentCalculator::addTangentsToMesh(plt->terrainMesh);

    plt->upload();
}
