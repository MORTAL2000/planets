#pragma once

// Standard Headers
#include <glm/mat4x4.hpp>

// Local Headers
#include "render_type.hpp"
#include "graphics/renderable.hpp"
#include "graphics/shader.hpp"
#include "graphics/camera.hpp"

class Renderer {
    protected:
        bool shadows = false;
        virtual void draw_objects(const std::vector<Renderable *> & renderables, glm::mat4 parent_model, Shader & shader, RenderType type, bool update_model); 
    public:
        Renderer();
        Renderer(Camera * camera);
        Camera * camera;
        virtual void render(double dt) = 0;
};
