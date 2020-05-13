#include "shadow_renderer.hpp"

#include <iostream>
#include <limits>

// Local headers
#include "scene.hpp"
#include "utils/resource_manager.hpp"

ShadowRenderer::ShadowRenderer(): Renderer(new Camera(2048, 2048)), buffer(2048, 2048)
{
    // camera = new Camera(150, 150, 90);
//    Renderer::camera = shadowCamera;
    camera->mode = ProjectionType::Orthographic;
    
    buffer.addDepthTexture(GL_LINEAR, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    sunDepthTexture = buffer.depthTexture;

    Shader shader = ResourceManager::LoadShader("shadow.vert", "empty.frag", "shadow");
}

void ShadowRenderer::render(double dt) {
    Shader shader = ResourceManager::GetShader("shadow");
    shader.enable();

    begin();

    glDisable(GL_BLEND);
    Universe universe = Globals::scene->getUniverse();
    draw_objects(universe.getRenderables(), glm::mat4(1.f), shader, RenderType::Terrain, true);
    glDisable(GL_BLEND);

    buffer.unbind();
}

void ShadowRenderer::begin()
{
    const Universe & universe = Globals::scene->getUniverse();

    float z_far = 0.1, z_near = std::numeric_limits<float>::max();

    for (Planet * planet : universe.getPlanets()) {
        float dist = glm::distance(planet->get_position(), camera->getPosition());

        z_far = max(z_far, dist + planet->config.radius * 2);
        z_near = min(z_near, dist - planet->config.radius * 2);
    }

    camera->lookAt(glm::vec3(0.f));

    CameraState state = camera->getState();
    state.position = universe.getSuns().front()->get_position();
    camera->moveTo(state, false);

    camera->calculate(z_near, z_far);

    buffer.bind();
    glClear(GL_DEPTH_BUFFER_BIT);
}
