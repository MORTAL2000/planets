#pragma once

// Local Headers
#include "renderer.hpp"
#include "common/planet.hpp"
#include "graphics/shader.hpp"

#include "graphics/texture.hpp"
#include "graphics/texture_array.hpp"

class TerrainRenderer: public Renderer {
    private:
        SharedTexture grass; //, ground, rock, sand, snow;
        SharedTexArray terrainTextures;
        void applyUniforms(Shader & shader);
    public: 
        TerrainRenderer();
        void render();
};
