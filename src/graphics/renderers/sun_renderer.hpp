#pragma once

// Local Headers
#include "renderer.hpp"
#include "common/planet.hpp"
#include "graphics/shader.hpp"

#include "graphics/texture.hpp"
#include "graphics/texture_array.hpp"

class SunRenderer: public Renderer {
    private:
        SharedTexture sunTexture;
        void applyUniforms(Shader & shader);
    public: 
        SunRenderer();
        void render(double dt);
};
