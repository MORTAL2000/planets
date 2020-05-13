#pragma once

#include "graphics/texture.hpp"
#include "utils/math_utils.h"
#include "graphics/frame_buffer.hpp"
#include "graphics/camera.hpp"
#include "graphics/renderers/renderer.hpp"

class ShadowRenderer: public Renderer
{
  public:

    inline static const mat4 BIAS_MATRIX = mat4(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    );

    SharedTexture sunDepthTexture;

    ShadowRenderer();

    void render(double dt);

  private:
    FrameBuffer buffer;
    void begin();
};