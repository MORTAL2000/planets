#pragma once

#include "graphics/texture.hpp"
#include "utils/math_utils.h"
#include "graphics/frame_buffer.hpp"
#include "graphics/camera.hpp"

class ShadowRenderer
{
  public:

    inline static const mat4 BIAS_MATRIX = mat4(
        0.5, 0.0, 0.0, 0.0,
        0.0, 0.5, 0.0, 0.0,
        0.0, 0.0, 0.5, 0.0,
        0.5, 0.5, 0.5, 1.0
    );

    SharedTexture sunDepthTexture;
    Camera sunCam;

    ShadowRenderer();

    void begin(const Camera &mainCam, const vec3 &sunDir);

    void end();

  private:

    FrameBuffer buffer;

};