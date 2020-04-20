#pragma once

// Local Heads
#include "rendering/camera.hpp"
#include "common/universe.hpp"
#include "utils/shader.hpp"

class Scene {
    private:
        Camera camera;
        Universe universe;

        Shader earth_shader;
    public:
        Scene();

        void resized();
        void draw(float dt);
        void update(float dt);
};