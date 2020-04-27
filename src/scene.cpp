#include "scene.hpp"

// Local Headers
#include "graphics/window_size.hpp"
#include "graphics/input/key_input.hpp"
#include "graphics/input/mouse_input.hpp"

Scene::Scene():
    camera(.1, 1000, 1, 1, 55), flyingCamera(&camera), planetCamera(&camera, universe.getPlanet()),
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

// void Scene::add_renderable(Renderable * renderable, Renderable * parent) {
//     if (parent) {
//         parent->add_child(renderable);
//     } else {
//         _objects.push_back(renderable);
//     }
// }

void Scene::draw(float dt) {

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
        planetCamera.update(dt, universe.getPlanet()); // planet camera movement
    else if (!KeyInput::pressed(GLFW_KEY_G))
        flyingCamera.update(dt); // flying camera movement

    // glm::vec3 sunDir = universe.calculateSunDirection();

    underwater_renderer.render();

    universe.update(dt);

    // sceneBuffer->bind();
    planet_renderer.render(universe.getPlanet());
    // sceneBuffer->unbind();

    // Post Processing
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
