#include "terrain_renderer.hpp"

#include "scene.hpp"
#include "graphics/window_size.hpp"
#include "graphics/camera.hpp"
#include "common/universe.hpp"
#include "utils/resource_manager.hpp"

#include "graphics/gl_error.hpp"

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

    
    glUniform3f(shader.uniform("sunDir"), camera.sunDir.x, camera.sunDir.y, camera.sunDir.z);
    glUniform1i(shader.uniform("backgroundTerrainLayer"), 0);
    glUniform4f(shader.uniform("terrainLayers"), 2, 4, 5, 6);
    glUniform4i(shader.uniform("hasNormal"), 1, 0, 0, 1); // (background must have normal)
    glUniform4i(shader.uniform("fadeBlend"), 0, 0, 0, 1);
    glUniform4f(shader.uniform("specularity"), .4, 0, 0, .6);
    glUniform4f(shader.uniform("textureScale"), 2.2, 1., 1., 1.5);

    terrainTextures->bind(0);
    glUniform1i(shader.uniform("terrainTextures"), 0);

    // shadowRenderer.sunDepthTexture->bind(1, terrainShader, "shadowBuffer");
    // mat4 shadowMatrix = ShadowRenderer::BIAS_MATRIX * shadowRenderer.sunCam.combined;
    // glUniformMatrix4fv(terrainShader.location("shadowMatrix"), 1, GL_FALSE, &((shadowMatrix)[0][0]));
    glUniformMatrix4fv(shader.uniform("viewTrans"), 1, GL_FALSE, &(camera.combined[0][0]));    
}