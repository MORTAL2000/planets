#include "path_renderer.hpp"

#include "scene.hpp"
#include "common/universe.hpp"
#include "utils/resource_manager.hpp"

#include "graphics/gl_error.hpp"
#include "render_type.hpp"

PathRenderer::PathRenderer() {
    Shader shader = ResourceManager::LoadShader("path.vert", "path.frag", "path");
    check_gl_error();
}

void PathRenderer::render(double dt) {
    Shader shader = ResourceManager::GetShader("path");
    shader.enable();
    check_gl_error();

    glDisable(GL_BLEND);
    check_gl_error();

    applyUniforms(shader);
    check_gl_error();

    Universe universe = Globals::scene->getUniverse();
    const std::vector<Renderable *> & renderables = universe.getRenderables();

    for(auto t=renderables.begin(); t!=renderables.end(); ++t) {
        (*t)->render(RenderType::Path);
        check_gl_error();
    }
}

void PathRenderer::applyUniforms(Shader & shader) {
    glUniformMatrix4fv( shader.uniform("MVP"), 1, GL_FALSE, glm::value_ptr(camera->combined)); // projection matrix
}
