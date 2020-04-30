#pragma once

// Local Headers
#include "common/planet.hpp"
#include "graphics/shader.hpp"

#include "graphics/texture_array.hpp"

class TerrainRenderer {
    private:
        // SharedTexture foamTexture, seaWaves;
        SharedTexArray terrainTextures;
        void applyUniforms(Shader & shader);
    public: 
        TerrainRenderer();
        void render();
};