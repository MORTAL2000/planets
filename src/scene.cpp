#include "scene.hpp"

// Local Headers
#include "utils/window_size.hpp"

Scene::Scene():
    camera(.1, 1000, 1, 1, 3.14f/4) {
    
    earth_shader.init_from_files("shaders/earth.vert", "shaders/earth.vert" );

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Scene::draw(float dt) {
    universe.update(dt);


}

void Scene::resized()
{
    camera.viewportWidth = WindowSize::widthPixels;
    camera.viewportHeight = WindowSize::heightPixels;
    if (sceneBuffer) delete sceneBuffer;
    sceneBuffer = new FrameBuffer(gu::widthPixels, gu::heightPixels, 4);
    sceneBuffer->addColorTexture(GL_RGB, GL_LINEAR, GL_LINEAR);
    sceneBuffer->addDepthTexture(GL_LINEAR, GL_LINEAR);
}