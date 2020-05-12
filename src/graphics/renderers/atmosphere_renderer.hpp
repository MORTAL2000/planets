#pragma once

// Local Headers
#include "renderer.hpp"
#include "common/planet.hpp"
#include "graphics/shader.hpp"

#include "graphics/texture.hpp"
#include "graphics/texture_array.hpp"

class AtmosphereRenderer: public Renderer {
    private:
        void applyUniforms(Shader & shader);
    public: 
        AtmosphereRenderer();
        void render(double dt);
};
