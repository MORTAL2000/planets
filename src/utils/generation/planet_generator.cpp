
#include "planet_generator.hpp"

// Standard Headers
#include <algorithm>

// Local Headers
#include "utils/math_utils.h"
#include "utils/math/interpolation.h"
#include "geometry/cubesphere.hpp"

#include "graphics/tangent_calculator.hpp"
#include "graphics/imgui/imgui.h"


void PlanetGenerator::ShowDebugWindow(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(430,450), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Planet Generator", p_open))
    {
        ImGui::End();
        return;
    }

    struct funcs
    {
        static void ConfigureNoiseLayer(int id, NoiseLayer & layer) {
            ImGui::PushID(id);
            ImGui::SliderInt("Number of Layers", &layer.numLayers, 0, 10);
            ImGui::SliderFloat("Strength", &layer.strength, 0.0f, 10.0f);
            ImGui::SliderFloat("Base Roughness" , &layer.baseRoughness, 0.0f, 10.0f);
            ImGui::SliderFloat("Roughness", &layer.roughness, 0.0f, 10.0f);
            ImGui::SliderFloat("Persistence", &layer.persistence, 0.0f, 10.0f);
            ImGui::SliderFloat("Min Value", &layer.minValue, -20.0f, 20.0f);
            ImGui::PopID();
        }
        
    };

    ImGui::Text("CONTINENTS");
    funcs::ConfigureNoiseLayer(0, CONTINENTS);

    ImGui::Text("MOUNTAINS");
    funcs::ConfigureNoiseLayer(1, MOUNTAINS);

    ImGui::End();
}

PlanetGenerator::PlanetGenerator() {
    planetNoise.SetNoiseType(FastNoise::Simplex);

    planetNoise.SetFrequency(1);

    deadGrassNoise.SetNoiseType(FastNoise::SimplexFractal);
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

float PlanetGenerator::calculateGrass(const glm::vec3 & unitSphere, VertexCharacteristics v) {
    if (v.height < GRASS_LEVEL || v.height > ROCK_LEVEL) return 0;

    float frequency = 6;
    float noise = 0.5 * (grassNoise.GetValue(unitSphere.x * frequency, unitSphere.y * frequency, unitSphere.z * frequency) + 1);

    float steepness = v.maxNeighbor - v.minNeighbor;
    float rockiness = clamp(steepness, 0.f, 1.f);

    // int maxDist = 1 + (int)(30 * abs(noise));
    // float dist = max(0.f, min(v.distToGrass, (float) maxDist + 3) - 3);
    return clamp(noise * (1-rockiness), 0.f, 1.f);
}

float PlanetGenerator::calculateDeadGrass(const glm::vec3 & unitSphere, VertexCharacteristics v) {
    if (v.height < GRASS_LEVEL || v.height > ROCK_LEVEL) return 0;
    // float noiseX = x + DEAD_GRASS_NOISE_OFFSET, noiseY = y + DEAD_GRASS_NOISE_OFFSET;

    float noise = deadGrassNoise.GetValue(unitSphere.x + DEAD_GRASS_NOISE_OFFSET, unitSphere.y + DEAD_GRASS_NOISE_OFFSET, unitSphere.z + DEAD_GRASS_NOISE_OFFSET);

    return clamp((noise - .2) * 3., 0.0, 1.0);
}

float PlanetGenerator::calculateRock2(const glm::vec3 & unitSphere, VertexCharacteristics v) {
    if (v.height < ROCK_LEVEL) return 0;

//    float noise = planetNoise.GetValue(unitSphere.x, unitSphere.y, unitSphere.z);

    return 1. - clamp<float>(v.maxNeighbor - v.height, 0., 1.);
}

float PlanetGenerator::calculateRock(const glm::vec3 & unitSphere, VertexCharacteristics v) {
    if (v.height < ROCK_LEVEL) return 0;

    float steepness = v.maxNeighbor - v.minNeighbor;

    float rockness = clamp<float>(steepness, 0., 1.) + (v.height > 3 ? 1 : 0);
    // rockness *= 1. - v.distToRock / 3.;
    return rockness;
}


void PlanetGenerator::addTextureMaps(Mesh * mesh) {
    VertAttributes &attrs = mesh->attributes;
    unsigned int posOffset = attrs.getOffset(VertAttributes::POSITION); 
    unsigned int texOffset = attrs.getOffset({"TEX_BLEND", 4});
    unsigned int yLevelOffset = attrs.getOffset({"Y_LEVEL", 1});

    std::vector<VertexCharacteristics> characteristics(mesh->nrOfVertices, {99999, -99999, 99999, 0, 0});

    // Calculate attributes with on neighbors 
    for (unsigned int i = 0; i < mesh->nrOfIndices; i += 3)
    {
        int vertI0 = mesh->indices[i], vertI1 = mesh->indices[i + 1], vertI2 = mesh->indices[i + 2];

        auto y0 = mesh->get<float>(vertI0, yLevelOffset),
            y1 = mesh->get<float>(vertI1, yLevelOffset),
            y2 = mesh->get<float>(vertI2, yLevelOffset);

        characteristics[vertI0].height = y0;
        characteristics[vertI1].height = y1;
        characteristics[vertI2].height = y2;

        // Rock isl->distToHeight(x, y, 1.5, 999, 3)

        float minHeight = min(y0, min(y1, y2));
        characteristics[vertI0].minNeighbor = min(characteristics[vertI0].minNeighbor, minHeight);
        characteristics[vertI1].minNeighbor = min(characteristics[vertI1].minNeighbor, minHeight);
        characteristics[vertI1].minNeighbor = min(characteristics[vertI1].minNeighbor, minHeight);

        float maxHeight = max(y0, max(y1, y2));
        characteristics[vertI0].maxNeighbor = max(characteristics[vertI0].maxNeighbor, maxHeight);
        characteristics[vertI1].maxNeighbor = max(characteristics[vertI1].maxNeighbor, maxHeight);
        characteristics[vertI1].maxNeighbor = max(characteristics[vertI1].maxNeighbor, maxHeight);
    }

    for (unsigned int i = 0; i < mesh->nrOfIndices; i ++) {
        int vertI = mesh->indices[i];

        auto pos = mesh->get<glm::vec3>(vertI, posOffset);

        VertexCharacteristics v = characteristics[vertI];

        glm::vec4 textureMap(0.f, 0.f, 0, 0);
        textureMap[GRASS_TEX] = calculateGrass(pos, v);
        textureMap[DEAD_GRASS_TEX] = calculateDeadGrass(pos, v);
        textureMap[ROCK_TEX] = calculateRock(pos, v);
        textureMap[ROCK2_TEX] = calculateRock2(pos, v);
        
        mesh->add<glm::vec4>(textureMap, vertI, texOffset);
    }
}

float PlanetGenerator::calculateElevation(const glm::vec3 & unitSphere) {
    float continent = simpleNoise(CONTINENTS, unitSphere);

    return continent + ridgedNoise(MOUNTAINS, unitSphere, 0.78) * continent;
}

void PlanetGenerator::generate(Planet *plt)
{
    planetNoise.SetSeed(time(0));

    VertAttributes terrainAttrs;
    unsigned int posOffset = terrainAttrs.add(VertAttributes::POSITION);
    terrainAttrs.add(VertAttributes::NORMAL);
    unsigned int uvOffset = terrainAttrs.add(VertAttributes::TEX_COORDS);
    terrainAttrs.add(VertAttributes::TANGENT);
    unsigned int texOffset = terrainAttrs.add({"TEX_BLEND", 4});
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
//    const float * normals = cubesphere.getNormals();
    const float * texCords = cubesphere.getTexCoords();

    std::vector<vec4> textureMap(nVertices, vec4());

    for (unsigned int i = 0; i < nVertices; i++) {
        // plt->waterMesh->set(glm::vec3(vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2]), i, posOffset);
        

        glm::vec3 pos(vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2]);
        // glm::vec3 normal = earth->mesh->get<glm::vec3>(vertI, normOffset);
        glm::vec3 normal = glm::normalize(pos);

        // Creating noise!
        float terrainHeight = calculateElevation(pos);

        plt->terrainMesh->set(terrainHeight, i, yLevelOffset);

        std::cout << "Height for pos: " << glm::to_string(pos) << " " << terrainHeight  + config.radius << std::endl;
        plt->terrainMesh->set<glm::vec3>(pos + ((terrainHeight + config.radius) * normal), i, posOffset);

        // Recalculate textures and normals
        // plt->terrainMesh->set(glm::vec3(normals[3 * i], normals[3 * i + 1], normals[3 * i + 2]), i, norOffset);
        plt->terrainMesh->set(glm::vec2(texCords[2 * i], texCords[2 * i + 1]), i, uvOffset);
        plt->terrainMesh->set(glm::vec4(0, 0, 0, 0), i, texOffset);
    }

    const unsigned int * indices = cubesphere.getIndices();
    for (unsigned int i = 0; i < nIndices; i++) {
        plt->terrainMesh->indices[i] = indices[i];
    }

    TangentCalculator::addTangentsToMesh(plt->terrainMesh);
    plt->terrainMesh->computeSmoothingNormals();

    addTextureMaps(plt->terrainMesh.get());

    plt->upload();
}
