#include "water_renderer.hpp"

#include "scene.hpp"
#include "graphics/window_size.hpp"
#include "graphics/camera.hpp"
#include "common/universe.hpp"
#include "utils/resource_manager.hpp"

#include "render_type.hpp"

WaterRenderer::WaterRenderer() {
    Shader shader = ResourceManager::LoadShader("water.vert", "water.frag", "water");
    check_gl_error();

    foamTexture = ResourceManager::LoadTexture("textures/tc_foam.dds", "foam");
    seaWaves = ResourceManager::LoadTexture("textures/sea_waves.dds", "seaWaves");
}

void WaterRenderer::render() {
    Shader shader = ResourceManager::GetShader("water");
    shader.enable();

    applyUniforms(shader);

    Universe universe = Globals::scene->getUniverse();

    // We don't need to update the models since the terrain renderer will already have done that
    draw_objects(universe.getRenderables(), glm::mat4(1.f), shader, RenderType::Water, false);
}

void WaterRenderer::applyUniforms(Shader & shader) {
    
    Camera camera = Globals::scene->getCamera();
    Universe universe = Globals::scene->getUniverse();

    // glUniformMatrix4fv(shader.uniform("viewProjection"), 1, GL_FALSE, &camera.combined[0][0]);
    glUniform1f(shader.uniform("time"), universe.getTime());
    glUniform2f(shader.uniform("scrSize"), WindowSize::widthPixels, WindowSize::heightPixels);
    glUniform3f(shader.uniform("camPos"), camera.position.x, camera.position.y, camera.position.z);
    glUniform3f(shader.uniform("sunDir"), camera.sunDir.x, camera.sunDir.y, camera.sunDir.z);

    // Bind Textures
    foamTexture->bind(0);
    glUniform1i(shader.uniform("foamTexture"), 0);

    seaWaves->bind(1);
    glUniform1i(shader.uniform("seaWaves"), 1);

    // Bind results from other buffers
    Globals::scene->underwater_renderer.underwaterBuffer.colorTexture->bind(2);
    glUniform1i(shader.uniform("underwaterTexture"), 2);

    Globals::scene->underwater_renderer.underwaterBuffer.depthTexture->bind(3);
    glUniform1i(shader.uniform("underwaterDepthTexture"), 3);

    // reflectionBuffer.colorTexture->bind(4,"reflectionTexture");
    // glUniform1i(shader.uniform("reflectionTexture"), 4);
}
