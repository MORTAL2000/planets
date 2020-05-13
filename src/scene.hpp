#pragma once

// Local Heads
#include "graphics/camera.hpp"
#include "graphics/controls/flying_camera.hpp"
#include "graphics/controls/planet_camera.hpp"

#include "graphics/input/key_input.hpp"
#include "graphics/renderable.hpp"
#include "graphics/frame_buffer.hpp"
#include "graphics/renderers/shadow_renderer.hpp"
#include "graphics/renderers/space_renderer.hpp"
#include "graphics/renderers/underwater_renderer.hpp"
#include "graphics/renderers/post_processing.hpp"
#include "common/universe.hpp"

class Scene {
    private:
        Camera camera;
        Universe universe;

        bool camPlanetMode = true;
        bool camDebugMode = true;
        void updateCamera(float dt);

        bool cursorToLonLat(const glm::vec3 & rayDir, vec2 &lonLat, float offset) const;


        std::vector<Renderable *> _objects;
    public:
        Scene();

        Planet * selected;
        FlyingCamera flyingCamera;
        PlanetCamera planetCamera;

        std::vector<Renderer *> renderers;
        
        ShadowRenderer * shadow_renderer;
        UnderwaterRenderer * underwater_renderer;
        PostProcessing * post_processing;
        FrameBuffer reflectionBuffer, *sceneBuffer = NULL;

        const Universe & getUniverse() { return universe; }

        void resize();
        void init();
        void draw(float dt);
        void update(float dt);

        // void add_renderable(Renderable * renderable, Renderable * parent = nullptr);
        friend class Renderer;
};

namespace Globals {
	extern Scene * scene;
}
