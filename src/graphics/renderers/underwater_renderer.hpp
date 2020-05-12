#pragma once

// Local Headers
#include "renderer.hpp"
#include "common/planet.hpp"
#include "graphics/shader.hpp"

#include "graphics/texture.hpp"
#include "graphics/frame_buffer.hpp"

class UnderwaterRenderer: public Renderer {
    private:
        SharedTexture caustics, sand;
        void applyUniforms(Shader & shader);
    public: 
        UnderwaterRenderer();
        FrameBuffer underwaterBuffer;
        void render(double dt);
};