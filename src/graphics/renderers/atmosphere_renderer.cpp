#include "atmosphere_renderer.hpp"

#include "scene.hpp"
#include "graphics/window_size.hpp"
#include "graphics/camera.hpp"
#include "common/universe.hpp"
#include "utils/resource_manager.hpp"

#include "graphics/gl_error.hpp"
#include "render_type.hpp"

AtmosphereRenderer::AtmosphereRenderer() {
    Shader shader = ResourceManager::LoadShader("atmosphere.vert", "atmosphere.frag", "atmosphere");

    check_gl_error();
}

void AtmosphereRenderer::render(double dt) {
    Shader shader = ResourceManager::GetShader("atmosphere");
    shader.enable();
    check_gl_error();

    glDepthMask(false);
    glEnable(GL_BLEND);
    check_gl_error();

    applyUniforms(shader);
    check_gl_error();

    Universe universe = Globals::scene->getUniverse();

    draw_objects(universe.getRenderables(), glm::mat4(1.f), shader, RenderType::Atmosphere, true);
    check_gl_error();

    glDepthMask(true);
    check_gl_error();
}

void AtmosphereRenderer::applyUniforms(Shader & shader) {
    
    Camera camera = Globals::scene->getCamera();

    glm::vec3 cameraPosition = camera.getState().position;

    // glUniform3fv(shader.uniform("camPos"), 1, glm::value_ptr(camera.getPosition()));

    float planetRadius = 150;
    float atmosphereRadius = 200;

    glm::mat4 atmoModel = glm::mat4(1.0f);
    glm::vec3 cP = glm::normalize(cameraPosition) * planetRadius;

    float lon = glm::degrees(atan2(cP.x, cP.z) + 150.f); // earth.longitude(cP.x, cP.z);
    float lat = glm::degrees(acos(cP.y / planetRadius));

    atmoModel = glm::rotate(atmoModel, -(lon + 90) * mu::DEGREES_TO_RAD, mu::Y);
    atmoModel = glm::rotate(atmoModel, lat * mu::DEGREES_TO_RAD, mu::X);

    glUniformMatrix4fv(shader.uniform("atmoModel"), 1, GL_FALSE, &atmoModel[0][0]);
    glUniform1f(shader.uniform("camDist"), glm::length(cameraPosition) - atmosphereRadius);


    glm::vec3 atmosphereSunDir = camera.sunDir;
    atmosphereSunDir = glm::rotate(atmosphereSunDir, (lon + 90) * mu::DEGREES_TO_RAD, mu::Y);
    atmosphereSunDir = glm::rotate(atmosphereSunDir, -lat * mu::DEGREES_TO_RAD, mu::X);
    glUniform3fv(shader.uniform("sunDir"), 1, glm::value_ptr(atmosphereSunDir));
}
