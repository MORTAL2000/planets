#pragma once

// Local Heads
#include "graphics/camera.hpp"
#include "graphics/controls/flying_camera.hpp"
#include "graphics/controls/planet_camera.hpp"

#include "graphics/renderable.hpp"
#include "graphics/frame_buffer.hpp"
#include "graphics/renderers/planet_renderer.hpp"
#include "graphics/renderers/underwater_renderer.hpp"
#include "common/universe.hpp"

class Scene {
    private:
        Camera camera;
        Universe universe;
        
        FlyingCamera flyingCamera;
        PlanetCamera planetCamera;

        bool camPlanetMode = true;

        FrameBuffer reflectionBuffer, *sceneBuffer = NULL;

        std::vector<Renderable *> _objects;
    public:
        Scene();
        
        UnderwaterRenderer underwater_renderer;
        PlanetRenderer planet_renderer;

        const Camera & getCamera() { return camera; }
        const Universe & getUniverse() { return universe; }

        void resize();
        void draw(float dt);
        void update(float dt);

        // void add_renderable(Renderable * renderable, Renderable * parent = nullptr);
};

namespace Globals {
	extern Scene * scene;
}