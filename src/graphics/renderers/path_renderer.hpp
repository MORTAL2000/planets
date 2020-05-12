#pragma once

// Local Headers
#include "renderer.hpp"
#include "common/planet.hpp"
#include "graphics/shader.hpp"

#include "graphics/texture.hpp"
#include "graphics/texture_array.hpp"

class PathRenderer: public Renderer {
    private:
        void applyUniforms(Shader & shader);
    public: 
        PathRenderer();
        void render(double dt);
};
