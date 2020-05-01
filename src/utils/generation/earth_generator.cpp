
#include "earth_generator.h"

#include <FastNoiseSIMD/FastNoiseSIMD.h>

// #include "landmass_shape_generator.h"
#include "utils/math_utils.h"
#include "utils/math/interpolation.h"
#include "geometry/cubesphere.hpp"

// #include "utils/json_model_loader.h"
#include "graphics/tangent_calculator.hpp"


namespace
{

void applyNoiseToMesh(Planet *earth) {
    FastNoiseSIMD* myNoise = FastNoiseSIMD::NewFastNoiseSIMD();
    PlanetConfig config = earth->config;
    // myNoise->SetFrequency(0.05f);
    // myNoise->SetNoiseType = FastNoise::Perlin;

    int noiseResolution = 128;

    std::cout << "Starting Noise Terrain Generator" << std::endl;

    float* noiseSet = myNoise->GetSimplexFractalSet(0, 0, 0, noiseResolution, noiseResolution, noiseResolution);
    // Get a set of 16 x 16 x 16 Simplex Fractal noise
    // float* noiseSet = myNoise->GetSimplexFractalSet(0, 0, 0, 16, 16, 16);    
    
    int index = 0;

    unsigned int posOffset = earth->mesh->attributes.getOffset(VertAttributes::POSITION);
    unsigned int normOffset = earth->mesh->attributes.getOffset(VertAttributes::NORMAL);

    float binSize = (noiseResolution - 1) / (2.f * config.radius);
    for (unsigned int vertI = 0; vertI < earth->mesh->nrOfVertices; vertI++)
    {
        // int vertI = earth->mesh->indices[index];

        glm::vec3 pos = earth->mesh->get<glm::vec3>(vertI, posOffset);
//        glm::vec3 normal = earth->mesh->get<glm::vec3>(vertI, normOffset);
        glm::vec3 normal = glm::normalize(pos - config.center);
        // Binning the vertices into noise chunks
        int i = (pos.x + config.radius) * binSize, 
            j = (pos.y + config.radius) * binSize, 
            k = (pos.z + config.radius) * binSize;

        assert(i >= 0 && i <= noiseResolution);
        assert(j >= 0 && j <= noiseResolution);
        assert(k >= 0 && k <= noiseResolution);

        // Creating noise!
        float noise = noiseSet[(i * noiseResolution * noiseResolution) + (j * noiseResolution) + k];
        float height = (noise * config.roughness + 1) * 0.5;

        // std::cout << "Applying height of: " << height << " for " << glm::to_string(pos) << std::endl;
     
        earth->mesh->set<glm::vec3>(pos + (height * normal), vertI, posOffset);
    }

    std::cout << "Finished Perlin Noise Terrain Generator" << std::endl;

    FastNoiseSIMD::FreeNoiseSet(noiseSet);
}

} // namespace

void generateEarth(Planet *earth)
{
    VertAttributes attrs;
    unsigned int posOffset = attrs.add(VertAttributes::POSITION);
    unsigned int norOffset = attrs.add(VertAttributes::NORMAL);
    unsigned int texOffset = attrs.add(VertAttributes::TEX_COORDS);

    PlanetConfig config = earth->config;

    Cubesphere sphere(config.radius, config.subdivision, false);

    unsigned int nIndices = sphere.getIndexCount();
    unsigned int nVertices = sphere.getVertexCount();
  
    earth->mesh = std::make_shared<Mesh>(earth->config.name, nVertices, nIndices, attrs);
    
    const float * vertices = sphere.getVertices();
    const float * normals = sphere.getNormals();
    const float * texCords = sphere.getTexCoords();
    
    for (unsigned int i = 0; i < nVertices; i++) {
        // earth->mesh->set(glm::vec3(vertices[i], vertices[i + 1], vertices[i + 2]), i, posOffset);
        earth->mesh->set(glm::vec3(vertices[3 * i], vertices[3 * i + 1], vertices[3 * i + 2]), i, posOffset);
        earth->mesh->set(glm::vec3(normals[3 * i], normals[3 * i + 1], normals[3 * i + 2]), i, norOffset);
        earth->mesh->set(glm::vec2(texCords[2 * i], texCords[2 * i + 1]), i, texOffset);
    }

    const unsigned int * indices = sphere.getIndices();
    for (unsigned int i = 0; i < nIndices; i++) {
        earth->mesh->indices[i] = indices[i];
    }


    applyNoiseToMesh(earth);


    earth->upload();
}
