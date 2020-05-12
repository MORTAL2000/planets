#pragma once

// Local Headers
#include "common/planet.hpp"
#include "graphics/camera.hpp"
#include "graphics/shader.hpp"
#include "graphics/cube_map.hpp"
#include "geometry/model.hpp"
#include "graphics/texture.hpp"
#include "graphics/texture_array.hpp"
#include "graphics/renderers/renderer.hpp"

// struct LensFlare
// {
//     int texture;
//     glm::vec4 color;
//     float scale, dist;
//     bool rotate;
// };

class SpaceRenderer: public Renderer
{
  public:
    SpaceRenderer();
    void render(double dt);

  private:

    // static const LensFlare flares[];

    // SharedTexture sunTexture;

    // SharedTexArray flareTextures;

    unsigned int skyboxVAO, skyboxVBO;
    SharedCubeMap cubeMap;

//    float lensFlareAlpha = 0;
};

