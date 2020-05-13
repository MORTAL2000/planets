#include "terrain_renderer.hpp"

#include "scene.hpp"
#include "graphics/window_size.hpp"
#include "graphics/camera.hpp"
#include "common/universe.hpp"
#include "utils/resource_manager.hpp"

#include "graphics/gl_error.hpp"
#include "render_type.hpp"

TerrainRenderer::TerrainRenderer() {
    Shader shader = ResourceManager::LoadShader("terrain.vert", "terrain.frag", "terrain");
    terrainTextures = ResourceManager::LoadTextureArray({
        "textures/tc_sand.dds",
        "textures/tc_sand_normal.dds",

        "textures/tc_rock.dds",
        "textures/tc_rock_normal.dds",

        "textures/tc_grass.dds",

        "textures/tc_grass_dead.dds",

        "textures/tc_rock2.dds",
        "textures/tc_rock2_normal.dds",
    }, "terrain_textures");

    // grass = ResourceManager::LoadTexture("textures/tc_grass.dds", "grass");
    // ground = ResourceManager::LoadTexture("textures/tc_grass_dead.dds", "ground");
    // rock = ResourceManager::LoadTexture("textures/tc_rock.dds", "rock");
    // sand = ResourceManager::LoadTexture("textures/tc_sand.dds", "sand");
    // snow = ResourceManager::LoadTexture("textures/snow512.tga", "snow");

    Renderer::shadows = true;

    check_gl_error();
}

void TerrainRenderer::render(double dt) {
    Shader shader = ResourceManager::GetShader("terrain");
    shader.enable();
    check_gl_error();

    glDisable(GL_BLEND);
    check_gl_error();

    applyUniforms(shader);
    check_gl_error();

    Universe universe = Globals::scene->getUniverse();

//    universe.getPlanet()->terrainMesh->render();

    draw_objects(universe.getRenderables(), glm::mat4(1.f), shader, RenderType::Terrain, true);
    
    // for (auto &planet : universe.getRenderables()) {
    //     planet->render(RenderType::Terrain);
    // }

    // Draw grids
    // for (auto isl : universe.getPlanet()->land)
    // {
    //     if (!isl->isInView) continue;
    //     isl->terrainMesh->render();
    // }
    check_gl_error();
}

void TerrainRenderer::applyUniforms(Shader & shader) {
    
    // Universe universe = Globals::scene->getUniverse();

    // Using an identity model for now
    // glUniformMatrix4fv(shader.uniform("viewProjection"), 1, GL_FALSE, &camera.combined[0][0]);
    
    // glUniform1f(shader.uniform("time"), universe.getTime());
    // glUniform2f(shader.uniform("scrSize"), WindowSize::widthPixels, WindowSize::heightPixels);
    // glUniform3f(shader.uniform("camPos"), camera.position.x, camera.position.y, camera.position.z);
    glUniform3f(shader.uniform("sunDir"), camera->sunDir.x, camera->sunDir.y, camera->sunDir.z);
    // glUniform3f(shader.uniform("planetCenter"), 0.f, 0.f, 0.f);
    // glUniform1f(shader.uniform("seaLevel"), 150.f);
    
    glUniform1i(shader.uniform("backgroundTerrainLayer"), 0);
    glUniform4f(shader.uniform("terrainLayers"), 2, 4, 5, 6);
    glUniform4i(shader.uniform("hasNormal"), 1, 0, 0, 1); // (background must have normal)
    glUniform4i(shader.uniform("fadeBlend"), 0, 0, 0, 1);
    glUniform4f(shader.uniform("specularity"), .4, 0, 0, .6);
    glUniform4f(shader.uniform("textureScale"), 2.2, 1., 1., 1.5);

    terrainTextures->bind(0);
    glUniform1i(shader.uniform("terrainTextures"), 0);

    // Bind results from other buffers
    Globals::scene->shadow_renderer->sunDepthTexture->bind(1);
    glUniform1i(shader.uniform("shadowBuffer"), 1);

    // grass->bind(2);
    // glUniform1i(shader.uniform("grassTexture"), 2);

    // ground->bind(2);
    // glUniform1i(shader.uniform("groundTexture"), 2);

    // rock->bind(3);
    // glUniform1i(shader.uniform("rockTexture"), 3);

    // sand->bind(4);
    // glUniform1i(shader.uniform("sandTexture"), 4);

    // snow->bind(5);
    // glUniform1i(shader.uniform("snowTexture"), 5); 
}
