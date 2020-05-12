#include "sun_renderer.hpp"

#include "scene.hpp"
#include "graphics/window_size.hpp"
#include "graphics/camera.hpp"
#include "common/universe.hpp"
#include "utils/resource_manager.hpp"

#include "graphics/gl_error.hpp"
#include "render_type.hpp"

SunRenderer::SunRenderer() {
    Shader shader = ResourceManager::LoadShader("sun.vert", "sun.frag", "sun");
    sunTexture = ResourceManager::LoadTexture("textures/sun/sun.jpg", "sun_textures");

    check_gl_error();
}

void SunRenderer::render(double dt) {
    Shader shader = ResourceManager::GetShader("sun");
    shader.enable();
    check_gl_error();

    // glDisable(GL_BLEND);
    check_gl_error();

    applyUniforms(shader);
    check_gl_error();

    Universe universe = Globals::scene->getUniverse();
    const std::vector<Sun * > & suns = universe.getSuns();
    std::vector<Renderable *> renderableSuns(suns.begin(), suns.end());
    
    draw_objects(renderableSuns, glm::mat4(1.f), shader, RenderType::Terrain, true);

    check_gl_error();
}

void SunRenderer::applyUniforms(Shader & shader) {

    sunTexture->bind(0);
    glUniform1i(shader.uniform("sunTexture"), 0);
}
