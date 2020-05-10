#pragma once

// Standard Headers
#include <glm/mat4x4.hpp>

// Local Headers
#include "render_type.hpp"
#include "graphics/renderable.hpp"
#include "graphics/shader.hpp"

class Renderer {
    protected:
        virtual void draw_objects(std::vector<Renderable *> renderables, glm::mat4 parent_model, Shader & shader, RenderType type, bool update_model); 
    public:
        Renderer();
        virtual void render() = 0;
};
