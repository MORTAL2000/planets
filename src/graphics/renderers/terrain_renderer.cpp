#include "terrain_renderer.hpp"

#include "scene.hpp"
#include "graphics/window_size.hpp"
#include "graphics/camera.hpp"
#include "common/universe.hpp"
#include "utils/resource_manager.hpp"

#include "graphics/gl_error.hpp"

TerrainRenderer::TerrainRenderer() {
    Shader shader = ResourceManager::LoadShader("terrain.vert", "terrain.frag", "terrain");
    // terrainTextures = ResourceManager::LoadTextureArray({
    //     "textures/tc_sand.dds",
    //     "textures/tc_sand_normal.dds",

    //     "textures/tc_rock.dds",
    //     "textures/tc_rock_normal.dds",

    //     "textures/tc_grass.dds",

    //     "textures/tc_grass_dead.dds",

    //     "textures/tc_rock2.dds",
    //     "textures/tc_rock2_normal.dds",
    // }, "terrain_textures");

    grass = ResourceManager::LoadTexture("textures/grass.tga", "grass");
    ground = ResourceManager::LoadTexture("textures/ground.tga", "ground");
    rock = ResourceManager::LoadTexture("textures/rock512.tga", "rock");
    sand = ResourceManager::LoadTexture("textures/sand256.tga", "sand");
    snow = ResourceManager::LoadTexture("textures/snow512.tga", "snow");

    check_gl_error();
}

void TerrainRenderer::render() {
    Shader shader = ResourceManager::GetShader("terrain");
    shader.enable();
    check_gl_error();

    glDisable(GL_BLEND);
    check_gl_error();

    applyUniforms(shader);
    check_gl_error();

    Universe universe = Globals::scene->getUniverse();

    universe.getPlanet()->mesh->render();

    // Draw grids
    // for (auto isl : universe.getPlanet()->land)
    // {
    //     if (!isl->isInView) continue;
    //     isl->terrainMesh->render();
    // }
    check_gl_error();
}

void TerrainRenderer::applyUniforms(Shader & shader) {
    
    Camera camera = Globals::scene->getCamera();
    // Universe universe = Globals::scene->getUniverse();
    glm::mat4 mvp = camera.combined;

    // Using an identity model for now
    glUniformMatrix4fv(shader.uniform("MVP"), 1, GL_FALSE, &mvp[0][0]);
    
    // glUniform1f(shader.uniform("time"), universe.getTime());
    // glUniform2f(shader.uniform("scrSize"), WindowSize::widthPixels, WindowSize::heightPixels);
    // glUniform3f(shader.uniform("camPos"), camera.position.x, camera.position.y, camera.position.z);
    // glUniform3f(shader.uniform("sunDir"), camera.sunDir.x, camera.sunDir.y, camera.sunDir.z);
    glUniform3f(shader.uniform("planetCenter"), 0.f, 0.f, 0.f);
    glUniform1f(shader.uniform("seaLevel"), 150.f);
    
    // glUniform1i(shader.uniform("backgroundTerrainLayer"), 0);
    // glUniform4f(shader.uniform("terrainLayers"), 2, 4, 5, 6);
    // glUniform4i(shader.uniform("hasNormal"), 1, 0, 0, 1); // (background must have normal)
    // glUniform4i(shader.uniform("fadeBlend"), 0, 0, 0, 1);
    // glUniform4f(shader.uniform("specularity"), .4, 0, 0, .6);
    // glUniform4f(shader.uniform("textureScale"), 2.2, 1., 1., 1.5);

    // terrainTextures->bind(0);
    // glUniform1i(shader.uniform("terrainTextures"), 0);

    grass->bind(1);
    glUniform1i(shader.uniform("grassTexture"), 1);

    ground->bind(2);
    glUniform1i(shader.uniform("groundTexture"), 2);

    rock->bind(3);
    glUniform1i(shader.uniform("rockTexture"), 3);

    sand->bind(4);
    glUniform1i(shader.uniform("sandTexture"), 4);

    snow->bind(5);
    glUniform1i(shader.uniform("snowTexture"), 5);

    // shadowRenderer.sunDepthTexture->bind(1, terrainShader, "shadowBuffer");
    // mat4 shadowMatrix = ShadowRenderer::BIAS_MATRIX * shadowRenderer.sunCam.combined;
    // glUniformMatrix4fv(terrainShader.location("shadowMatrix"), 1, GL_FALSE, &((shadowMatrix)[0][0]));
    // glUniformMatrix4fv(shader.uniform("viewTrans"), 1, GL_FALSE, &(camera.combined[0][0]));    
}