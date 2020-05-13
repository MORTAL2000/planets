#pragma once

// Local Headers
#include "common/planet.hpp"
#include "graphics/camera.hpp"
#include "graphics/texture.hpp"
#include "graphics/renderers/renderer.hpp"
#include "utils/resource_manager.hpp"

struct Cloud
{
    float lon, lat, speed, timeSinceSpawn, timeToDespawn;
    int spawnPoints;
};

class CloudRenderer: public Renderer
{
  public:
    CloudRenderer();

    void render(double dt);

  private:
    SharedMesh quad;
    SharedTexture noiseTex;
    std::vector<Cloud> clouds;

    void render(Planet * plt, double time, float deltaTime);
};

