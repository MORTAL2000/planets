#pragma once

// Local Headers
#include "renderer.hpp"
#include "common/planet.hpp"
#include "graphics/shader.hpp"

#include "graphics/texture.hpp"

class WaterRenderer: public Renderer {
    private:
        SharedTexture foamTexture, seaWaves;
        void applyUniforms(Shader & shader);
    public: 
        WaterRenderer();
        void render(double dt);
};