#pragma once

// Local Headers
// #include "common/planet.hpp"
#include "graphics/shader.hpp"

// #include "graphics/texture.hpp"

class PostProcessing {
    private:
        void applyUniforms(Shader & shader);
    public: 
        PostProcessing();
        void render();
};