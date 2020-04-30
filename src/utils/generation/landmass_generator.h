#ifndef ISLAND_GENERATOR_H
#define ISLAND_GENERATOR_H

#include <functional>
#include "common/land_mass.hpp"

typedef std::function<void(LandMass *)> TerrainGenerator;
typedef std::function<void(LandMass *)> TextureMapper;

class IslandGenerator
{

  public:
    IslandGenerator(int width, int height, Planet *plt, TerrainGenerator terrainGenerator, TextureMapper textureMapper);

    LandMass *generateEssentials();

    void finishGeneration();

  private:
    LandMass *isl = nullptr;
    TerrainGenerator terrainGenerator;
    TextureMapper textureMapper;

    bool generated = false;

    void reset(int width, int height, Planet *plt);

    bool tryToGenerate();

    void initVertexPositions();

};

#endif
