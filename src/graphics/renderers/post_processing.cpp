#include "post_processing.hpp"

#include "scene.hpp"
#include "graphics/window_size.hpp"
// #include "graphics/camera.hpp"
// #include "common/universe.hpp"
#include "utils/resource_manager.hpp"

PostProcessing::PostProcessing() {
    Shader shader = ResourceManager::LoadShader("post_processing.vert", "post_processing.frag", "post_processing");
    check_gl_error();
}

void PostProcessing::render() {
    glEnable(GL_BLEND);

    Shader shader = ResourceManager::GetShader("post_processing");
    shader.enable();

    applyUniforms(shader);

    glDisable(GL_DEPTH_TEST);
    Mesh::getQuad()->render();
}

void PostProcessing::applyUniforms(Shader & shader) {
    
//    glUniform1f(shader.uniform("zoomEffect"), Globals::scene->planetCamera.zoomVelocity / 2.);
//    glUniform1f(shader.uniform("zoom"), Globals::scene->planetCamera.actualZoom);
    glUniform2f(shader.uniform("resolution"), WindowSize::widthPixels, WindowSize::heightPixels);

    // Bind results from other buffers
    Globals::scene->sceneBuffer->colorTexture->bind(0);
    glUniform1i(shader.uniform("scene"), 0);

    Globals::scene->sceneBuffer->depthTexture->bind(1);
    glUniform1i(shader.uniform("sceneDepth"), 1);
}
