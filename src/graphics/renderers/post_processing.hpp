#pragma once

// Local Headers
#include "graphics/shader.hpp"
#include "graphics/renderers/renderer.hpp"
#include "graphics/texture_array.hpp"

struct LensFlare
{
    int texture;
    glm::vec4 color;
    float scale, dist;
    bool rotate;
};

class PostProcessing: public Renderer {
    private:
        static const LensFlare flares[];
        SharedTexArray flareTextures;
        float lensFlareAlpha = 0;

        void renderFlares(Shader & shader);
        void applyUniforms(Shader & shader);
    public: 
        PostProcessing();
        void render(double dt);
};
