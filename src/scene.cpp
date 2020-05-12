#include "scene.hpp"

// Standard Headers
#include <limits>
#include <glm/gtx/intersect.hpp>

// Local Headers
#include "graphics/window_size.hpp"
#include "graphics/input/key_input.hpp"
#include "graphics/input/mouse_input.hpp"

#include "graphics/renderers/path_renderer.hpp"
#include "graphics/renderers/terrain_renderer.hpp"
#include "graphics/renderers/water_renderer.hpp"
#include "graphics/renderers/atmosphere_renderer.hpp"
#include "graphics/renderers/cloud_renderer.hpp"
#include "graphics/renderers/sun_renderer.hpp"

Scene::Scene():
    camera(1, 1, 55), universe(), flyingCamera(&camera), planetCamera(&camera),
    reflectionBuffer(512, 512) {

    reflectionBuffer.addColorTexture(GL_RGB, GL_LINEAR, GL_LINEAR);
    reflectionBuffer.addDepthBuffer();

    selected = universe.getPlanets()[0];
    camera.lookAt(glm::vec3(0.f));

    flyingCamera.speedMultiplier = 100;
    planetCamera.speedMultiplier = 100;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // check_gl_error();

    // add_renderable(universe.getPlanet());

    check_gl_error();
    MouseInput::setLockedMode(!camPlanetMode);

    // Setup renderers
    renderers.push_back(new SunRenderer());
    renderers.push_back(new TerrainRenderer());
    renderers.push_back(new WaterRenderer());
    renderers.push_back(new SpaceRenderer());
    renderers.push_back(new AtmosphereRenderer());
    renderers.push_back(new CloudRenderer());
    renderers.push_back(new PathRenderer());
}

void Scene::update(float dt) {
    draw(dt);

    if (MouseInput::justPressed(GLFW_MOUSE_BUTTON_LEFT)) {

        std::cout << "Clicked!" << std::endl;

        glm::vec3 rayDir = camera.getCursorRayDirection();

        float nearestDist = std::numeric_limits<float>::max();
        Planet * nearest = nullptr;
        
        for (Planet * planet : universe.getPlanets()) {
            float dist;
            if (glm::intersectRaySphere(camera.getPosition(), rayDir, planet->get_position(), planet->config.radius * planet->config.radius, dist)) {
                if (dist < nearestDist) {
                    nearestDist = dist;
                    nearest = planet;
                }
            } 
        }

        if (nearest != nullptr) {
            selected = nearest;
            std::cout << "Selected: " << selected->name << std::endl;
        }
    }


    universe.update(dt);
}

void Scene::draw(float dt) {
    check_gl_error();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    check_gl_error();

    updateCamera(dt);

    glm::vec3 sunPosition = universe.getSuns().front()->get_position();
    camera.sunDir = glm::normalize(sunPosition - camera.getPosition()); // universe.calculateSunDirection(planetCamera.lat, planetCamera.lon, planetCamera.actualZoom);
   
    glEnable(GL_BLEND);

    // Render shadows
    underwater_renderer.render(dt);
    check_gl_error();

    sceneBuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    check_gl_error();

    for (Renderer * renderer : renderers)
    {
        renderer->render(dt);
        check_gl_error();
    }
    
    check_gl_error();

    sceneBuffer->unbind();
    // check_gl_error();

    glEnable(GL_BLEND);

    // Post Processing
    post_processing.render();
    check_gl_error();
}

void Scene::updateCamera(float dt) {
    if (KeyInput::justPressed(GLFW_KEY_P))
        MouseInput::setLockedMode(false);
    else if (KeyInput::justReleased(GLFW_KEY_P) && !camPlanetMode)
        MouseInput::setLockedMode(true);
        
    if (KeyInput::justPressed(GLFW_KEY_C))
    {
        camPlanetMode = !camPlanetMode;
        MouseInput::setLockedMode(!camPlanetMode);
    }

    if (camPlanetMode) {
        // Check if I should animate to the new planet
        if (planetCamera.planet != selected) {
            // camera.lookAt(selected->get_position());
            planetCamera.planet = selected;

            camera.moveTo(planetCamera.calculate(dt), true);
        } else {
            camera.moveTo(planetCamera.calculate(dt), false);
        }
    }
    else if (!KeyInput::pressed(GLFW_KEY_P))
        camera.moveTo(flyingCamera.calculate(dt), false); // flying camera movement

    float z_far = 0, z_near = 0.1;

    for (Sun * sun : universe.getSuns()) {
        float dist = glm::distance(sun->get_position(), camera.getPosition());

        z_far = max(z_far, 2 * dist + sun->shape.radius);
        // z_near = min(z_near, dist - planet->config.radius);
    }

    for (Planet * planet : universe.getPlanets()) {
        float dist = glm::distance(planet->get_position(), camera.getPosition());

        z_far = max(z_far, 2 * dist + planet->config.radius);
        // z_near = min(z_near, dist - planet->config.radius);
    }

    assert(z_far > z_near);

    camera.calculate(z_near, z_far, dt);

    if (camDebugMode) camera.ShowDebugWindow(&camDebugMode);
}

void Scene::resize()
{
    camera.viewportWidth = WindowSize::widthPixels;
    camera.viewportHeight = WindowSize::heightPixels;

    if (sceneBuffer) delete sceneBuffer;
    sceneBuffer = new FrameBuffer(WindowSize::widthPixels, WindowSize::heightPixels, 0);
    sceneBuffer->addColorTexture(GL_RGB, GL_LINEAR, GL_LINEAR);
    sceneBuffer->addDepthTexture(GL_LINEAR, GL_LINEAR);
}
