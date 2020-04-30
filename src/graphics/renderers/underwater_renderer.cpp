#include "underwater_renderer.hpp"

#include "scene.hpp"
#include "graphics/window_size.hpp"
#include "graphics/camera.hpp"
#include "common/universe.hpp"
#include "utils/resource_manager.hpp"

UnderwaterRenderer::UnderwaterRenderer(): underwaterBuffer(1024, 1024) {
    underwaterBuffer.addColorTexture(GL_RGBA, GL_LINEAR, GL_LINEAR);
    underwaterBuffer.addDepthTexture(GL_LINEAR, GL_LINEAR);

    Shader shader = ResourceManager::LoadShader("terrain_caustics.vert", "terrain_caustics.frag", "caustics");
    check_gl_error();

    caustics = ResourceManager::LoadTexture("textures/tc_caustics.dds", "caustics");
    sand = ResourceManager::LoadTexture("textures/tc_sand.dds", "sand");
}

void UnderwaterRenderer::render() {
    Universe universe = Globals::scene->getUniverse();

    underwaterBuffer.bind();
    glEnable(GL_BLEND);

    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    Shader shader = ResourceManager::GetShader("caustics");    
    shader.enable();

    applyUniforms(shader);
    
//    for (auto land : universe.getPlanet()->land)
//    {
//        if (!land->isInView) continue;
//        land->terrainMesh->render();
//    }

    // render waves to alpha channel of underwater

    underwaterBuffer.unbind();
}

void UnderwaterRenderer::applyUniforms(Shader & shader) {
    
    const Camera & camera = Globals::scene->getCamera();
    Universe universe = Globals::scene->getUniverse();

    // glUniformMatrix4fv(shader.uniform("MVP"), 1, GL_FALSE, &mvp[0][0]);
    glUniform1f(shader.uniform("time"), universe.getTime());
    // glUniform2f(shader.uniform("scrSize"), WindowSize::widthPixels, WindowSize::heightPixels);
    // glUniform3f(shader.uniform("camPos"), camera.position.x, camera.position.y, camera.position.z);
    glUniform3f(shader.uniform("sunDir"), camera.sunDir.x, camera.sunDir.y, camera.sunDir.z);

    glUniformMatrix4fv(shader.uniform("viewTrans"), 1, GL_FALSE, &camera.combined[0][0]);

    // Bind Textures
    caustics->bind(0);
    glUniform1i(shader.uniform("causticsSheet"), 0);

    sand->bind(1);
    glUniform1i(shader.uniform("terrainTexture"), 1);
}
