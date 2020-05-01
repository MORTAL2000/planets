#pragma once

// Local Headers
#include "common/planet.hpp"
#include "graphics/shader.hpp"

#include "graphics/texture.hpp"

class WaterRenderer {
    private:
        SharedTexture foamTexture, seaWaves;
        void applyUniforms(Shader & shader);
    public: 
        WaterRenderer();
        void render();
};