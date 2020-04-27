#pragma once

// Local Headers
#include "common/planet.hpp"
#include "graphics/camera.hpp"
#include "graphics/shader.hpp"
// #include "graphics/cube_map.hpp"
#include "geometry/model.hpp"
#include "graphics/texture.hpp"
// #include "graphics/texture_array.hpp"

struct LensFlare
{
    int texture;
    glm::vec4 color;
    float scale, dist;
    bool rotate;
};

class SpaceRenderer
{
  public:
    SpaceRenderer();

    void renderBox(const glm::vec3 &sunDir, const Camera &cam, float zoom);

    // void renderSun(const glm::vec3 &sunDir, const Camera &cam, SharedTexture depth, float time, const Planet &plt);

  private:

    static const LensFlare flares[];

    // SharedTexture sunTexture;

    // SharedTexArray flareTextures;

    // SharedMesh cube;
    // SharedCubeMap cubeMap;
    // ShaderProgram cubeMapShader, sunShader, flareShader;

    float lensFlareAlpha = 0;
};

