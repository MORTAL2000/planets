#include "scene.hpp"

// Standard Headers
#include <limits>

// Local Headers
#include "graphics/window_size.hpp"
#include "graphics/input/key_input.hpp"
#include "graphics/input/mouse_input.hpp"

Scene::Scene():
    camera(1, 1, 55), universe(), flyingCamera(&camera), planetCamera(&camera),
    reflectionBuffer(512, 512) {

    reflectionBuffer.addColorTexture(GL_RGB, GL_LINEAR, GL_LINEAR);
    reflectionBuffer.addDepthBuffer();

    camera.position = glm::vec3(0, 0, 300);
    camera.lookAt(glm::vec3(0.f), camera.up);

    flyingCamera.speedMultiplier = 100;
//    planetCamera.speedMultiplier = 100;

    // phong_renderer = new PhongRenderer();

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
    renderers.push_back(new TerrainRenderer());
    renderers.push_back(new WaterRenderer());
}

void Scene::draw(float dt) {
    check_gl_error();

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    check_gl_error();

    updateCamera(dt);

    camera.sunDir = glm::vec3(1, 0, 0); // universe.calculateSunDirection(planetCamera.lat, planetCamera.lon, planetCamera.actualZoom);
   
    glEnable(GL_BLEND);

    // Render shadows
    underwater_renderer.render();
    check_gl_error();

    // sceneBuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    check_gl_error();

    for (Renderer * renderer : renderers)
    {
        renderer->render();
        check_gl_error();
    }

    // space_renderer.renderBox();
    // atmosphere
    // cloud_renderer.render(time, newDeltaTime, cam, sunDir, &lvl->earth);
    // glDisable(GL_BLEND);

    universe.update(dt);
    check_gl_error();

    // sceneBuffer->unbind();
    // check_gl_error();

    // glEnable(GL_BLEND);

    // // Post Processing
    // post_processing.render();
    // check_gl_error();

    // space_renderer.renderSun();
    // glEnable(GL_DEPTH_TEST);
    // check_gl_error();
}

void Scene::updateCamera(float dt) {
    if (KeyInput::justPressed(GLFW_KEY_G))
        MouseInput::setLockedMode(false);
    else if (KeyInput::justReleased(GLFW_KEY_G) && !camPlanetMode)
        MouseInput::setLockedMode(true);

    if (KeyInput::justPressed(GLFW_KEY_C))
    {
        camPlanetMode = !camPlanetMode;
        MouseInput::setLockedMode(!camPlanetMode);
    }

    if (camPlanetMode) 
        planetCamera.update(dt); // planet camera movement
    else if (!KeyInput::pressed(GLFW_KEY_G))
        flyingCamera.update(dt); // flying camera movement

    float z_far = 0,
    //     z_near = std::numeric_limits<float>::max();

        // float z_far = 2000;
         z_near = 0.1;
    
    // Base it off of bounding boxes
    // for (Renderable * renderable : universe.getRenderables()) {
    //     renderable
    // }

    for (Planet * planet : universe.getPlanets()) {
        float dist = glm::distance(planet->get_position(), camera.position);

        z_far = max(z_far, dist + planet->config.radius + 10.f);
        // z_near = min(z_near, dist - planet->config.radius);
    }

    assert(z_far > z_near);

    camera.calculate(z_near, z_far);

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
