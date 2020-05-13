
#include "space_renderer.hpp"
#include "graphics/vert_buffer.hpp"
// #include "utils/json_model_loader.h"
#include "utils/math_utils.h"
#include "graphics/window_size.hpp"
// #include "gu/game_utils.h"
#include "common/planet.hpp"
#include "scene.hpp"
#include "utils/resource_manager.hpp"

float skyboxVertices[] = {
    // positions          
    -1.0f,  1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,

    -1.0f, -1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
    -1.0f, -1.0f,  1.0f,

    -1.0f,  1.0f, -1.0f,
     1.0f,  1.0f, -1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f,  1.0f,
    -1.0f,  1.0f, -1.0f,

    -1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f
};

SpaceRenderer::SpaceRenderer()
{
    Shader space_cube = ResourceManager::LoadShader("space_box.vert", "space_box.frag", "space_cube");

    cubeMap = ResourceManager::LoadCubeMap({
        "textures/space_cubemap/right.dds",
        "textures/space_cubemap/left.dds",
        "textures/space_cubemap/top.dds",
        "textures/space_cubemap/bottom.dds",
        "textures/space_cubemap/front.dds",
        "textures/space_cubemap/back.dds",
    }, "space_cubemap");

    // skybox VAO
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
}

void SpaceRenderer::render(double dt)
{
    Shader shader = ResourceManager::GetShader("space_cube");

    // glDisable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    
    shader.enable();

    glUniform1f(shader.uniform("atmosphere"), Globals::scene->planetCamera.atmosphereTilt);

    float daylight = 1.f - pow(((glm::dot(camera->getState().direction, camera->sunDir) + 1) * 0.5f), 2.f);
    glUniform1f(shader.uniform("daytime"), daylight);
    
    glm::mat4 view = glm::mat4(glm::mat3(camera->view));

    glUniformMatrix4fv(shader.uniform("view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(shader.uniform("projection"), 1, GL_FALSE, &camera->projection[0][0]);

    // skybox cube
    glBindVertexArray(skyboxVAO);

    cubeMap->bind(0);
    glUniform1i(shader.uniform("cubemap"), 0);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}
