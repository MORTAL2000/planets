#pragma once

// Local Headers
#include "common/planet.hpp"
#include "graphics/shader.hpp"

#include "graphics/texture.hpp"

class PlanetRenderer {
    private:
        SharedTexture foamTexture, seaWaves;
        void applyUniforms(Shader & shader);
    public: 
        PlanetRenderer();
        void render(Planet * planet);
};