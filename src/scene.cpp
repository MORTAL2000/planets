#include "scene.hpp"

// Local Headers
#include "graphics/window_size.hpp"
#include "graphics/input/key_input.hpp"
#include "graphics/input/mouse_input.hpp"

Scene::Scene():
    universe("pre-generated-level.save"), camera(.1, 1000, 1, 1, 55), flyingCamera(&camera), planetCamera(&camera, universe.getPlanet()),
    reflectionBuffer(512, 512) {

    reflectionBuffer.addColorTexture(GL_RGB, GL_LINEAR, GL_LINEAR);
    reflectionBuffer.addDepthBuffer();

    camera.position = glm::vec3(0, 0, 300);
    camera.lookAt(glm::vec3(0.f), camera.up);
    camera.update();

    flyingCamera.speedMultiplier = 100;

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
}

void Scene::draw(float dt) {
    check_gl_error();
    if (KeyInput::justPressed(GLFW_KEY_G))
        MouseInput::setLockedMode(false);
    else if (KeyInput::justReleased(GLFW_KEY_G) && !camPlanetMode)
        MouseInput::setLockedMode(true);

    if (KeyInput::justPressed(GLFW_KEY_C))
    {
        camPlanetMode = !camPlanetMode;
        MouseInput::setLockedMode(!camPlanetMode);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    check_gl_error();

    if (camPlanetMode) 
        planetCamera.update(dt); // planet camera movement
    else if (!KeyInput::pressed(GLFW_KEY_G))
        flyingCamera.update(dt); // flying camera movement

    camera.sunDir = universe.calculateSunDirection(planetCamera.lat, planetCamera.lon, planetCamera.actualZoom);
   

    glEnable(GL_BLEND);

    // float prevCamFar = camera.z_far;
    // camera.z_far = planetCamera.horizonDistance - 10.;
    // cam.update();

    // Render shadows
    underwater_renderer.render();
    check_gl_error();

    // sceneBuffer->bind();
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    check_gl_error();

    terrain_renderer.render();
    check_gl_error();

    glEnable(GL_BLEND);
    planet_renderer.render(universe.getPlanet());
    check_gl_error();

    // space_renderer.renderBox();
    // atmosphere
    // cloud_renderer.render(time, newDeltaTime, cam, sunDir, &lvl->earth);
    glDisable(GL_BLEND);

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

void Scene::resize()
{
    camera.viewportWidth = WindowSize::widthPixels;
    camera.viewportHeight = WindowSize::heightPixels;

    if (sceneBuffer) delete sceneBuffer;
    sceneBuffer = new FrameBuffer(WindowSize::widthPixels, WindowSize::heightPixels, 0);
    sceneBuffer->addColorTexture(GL_RGB, GL_LINEAR, GL_LINEAR);
    sceneBuffer->addDepthTexture(GL_LINEAR, GL_LINEAR);
}
