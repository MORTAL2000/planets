#pragma once

// Local Headers
#include "common/planet.hpp"
#include "graphics/shader.hpp"

#include "graphics/texture.hpp"
#include "graphics/frame_buffer.hpp"

class UnderwaterRenderer {
    private:
        SharedTexture caustics, sand;
        void applyUniforms(Shader & shader);
    public: 
        UnderwaterRenderer();
        FrameBuffer underwaterBuffer;
        void render();
};