
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
    // sunTexture = ResourceManager::LoadTexture("textures/sun/sun.dds", "sun");
    Shader space_cube = ResourceManager::LoadShader("space_box.vert", "space_box.frag", "space_cube");

    cubeMap = ResourceManager::LoadCubeMap({
        "textures/space_cubemap/right.dds",
        "textures/space_cubemap/left.dds",
        "textures/space_cubemap/top.dds",
        "textures/space_cubemap/bottom.dds",
        "textures/space_cubemap/front.dds",
        "textures/space_cubemap/back.dds",
    }, "space_cubemap");

//    flareTextures = ResourceManager::LoadTextureArray({
//        "textures/sun/flare1.dds",
//        "textures/sun/flare0.dds",
//        "textures/sun/flare3.dds",
//        "textures/sun/flare4.dds",
//        "textures/sun/flare5.dds"
//    }, "flare_textures");

    // cube = ResourceManager::LoadModel("models/cube.obj", "cube")->meshes[0];

    // skybox VAO
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    // Shader sun_shader = ResourceManager::LoadShader("sun_and_flare.vert", "sun.frag", "sun_shader");
    // Shader flare_shader = ResourceManager::LoadShader("sun_and_flare.vert", "flare.frag", "flare_shader");
}

void SpaceRenderer::render(double dt)
{
    Camera camera = Globals::scene->getCamera();
    Shader shader = ResourceManager::GetShader("space_cube");

    // glDisable(GL_BLEND);
    glDepthFunc(GL_LEQUAL);
    
    shader.enable();

    glUniform1f(shader.uniform("atmosphere"), Globals::scene->planetCamera.atmosphereTilt);

    float daylight = 1.f - pow(((glm::dot(camera.getState().direction, camera.sunDir) + 1) * 0.5f), 2.f);
    glUniform1f(shader.uniform("daytime"), daylight);
    
    glm::mat4 view = glm::mat4(glm::mat3(camera.view));  
    // mat4 view = glm::rotate(camera.combined, (float) -atan2(camera.sunDir.z, camera.sunDir.x) + mu::PI * .5f, mu::Y);

    glUniformMatrix4fv(shader.uniform("view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(shader.uniform("projection"), 1, GL_FALSE, &camera.projection[0][0]);

    // skybox cube
    glBindVertexArray(skyboxVAO);

    cubeMap->bind(0);
    glUniform1i(shader.uniform("cubemap"), 0);

    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glBindVertexArray(0);
    glDepthFunc(GL_LESS); // set depth function back to default
}

// const LensFlare SpaceRenderer::flares[] = {
//     {4, vec4(1), 1, 1, false},

//     {1, vec4(1, 1, 1, .35), 1, -.8, true},

//     {3, vec4(.7, .8, .9, .05), .25, -1.6, false},
//     {3, vec4(.9, .8, .4, .1), .38, -1.5, false},
//     {3, vec4(.7, .9, .8, .15), .45, -1.4, false},

//     {3, vec4(.9, .9, .7, .1), .18, -.75, false},

//     {0, vec4(.9, .9, .7, .7), .05, -.74, false},
//     {0, vec4(1), .015, -.75, false},

//     {0, vec4(.2, .3, 2, .7), .05, -.8, false},
//     {0, vec4(1), .015, -.81, false},

//     {3, vec4(.9, .9, .7, .07), .5, -.5, false},
//     {3, vec4(.9, .9, .7, .03), .8, -.54, false},

//     {3, vec4(.1, 1, .2, .2), .1, -.5, false},
//     {3, vec4(.1, 1, .2, .2), .09, -.47, false},
//     {0, vec4(.7, 1, .7, 1), .008, -.48, false},

//     {0, vec4(.1, .7, 1, .4), .06, .1, false},
//     {0, vec4(.7, 1, .7, 1), .008, .11, false},

//     {3, vec4(.8, .8, .3, .2), .06, .23, false},
//     {0, vec4(1, 1, .7, 1), .008, .225, false},

//     {0, vec4(.6, 1, .8, 1), .1, .23, false},

//     {2, vec4(1, .3, .1, .2), .5, 1.03, false},
//     {3, vec4(.6, .4, 1, .15), .4, 1.55, false},

//     {0, vec4(.2, 1, .5, .3), .03, 1.8, false},
//     {0, vec4(.2, 1, .5, 1), .008, 1.81, false},
// };

// void SpaceRenderer::renderSun()
// {
//     Camera camera = Globals::scene->getCamera();
//     Universe universe = Globals::scene->getUniverse();
//     Shader sun_shader = ResourceManager::GetShader("sun_shader");
//     sun_shader.enable();

//     sunTexture->bind(0);
//     glUniform1i(sun_shader.uniform("sun"), 0);

//     Globals::scene->sceneBuffer->depthTexture->bind(1);
//     glUniform1i(sun_shader.uniform("depthTex"), 1);

//     mat4 modelT = glm::translate(glm::translate(glm::mat4(1.f), camera.getPosition()), camera.sunDir * vec3(5));
//     modelT = glm::rotate(modelT, (float) -atan2(camera.sunDir.z, camera.sunDir.x) + mu::PI * -.5f, mu::Y);

//     glUniformMatrix4fv(sun_shader.uniform("mvp"), 1, GL_FALSE, &(camera.combined * modelT)[0][0]);
//     glUniform2f(sun_shader.uniform("scrSize"), WindowSize::widthPixels, WindowSize::heightPixels);
//     glUniform1f(sun_shader.uniform("time"), universe.getTime());
//     glDisable(GL_DEPTH_TEST);
//     Mesh::getQuad()->render();

//     // lensflare:
//     float lensFlareA = 1;

//     int steps = 50;

//     for (int i = 0; i < steps; i++)
//     {
//         vec3 sd = glm::rotate(camera.sunDir, (float) (.05 * (((float) i - steps * .5) / steps)), mu::Y);
//         vec3 sunPos = camera.getPosition() + sd;

//         bool inViewport = false;
//         camera.project(sunPos, inViewport);
        
// //        if (!inViewport || universe.getPlanet()->rayIntersection(camera.position, sd, NULL, NULL))
// //            lensFlareA -= 1.0 / steps;

//     }
//     lensFlareA = pow(lensFlareA, 2.);
//     lensFlareA = lensFlareAlpha = (lensFlareA + lensFlareAlpha) * .5;

//     vec2 screenSunPos = camera.project(camera.getPosition() + camera.sunDir);

//     lensFlareA *= 1.4 - length(screenSunPos);
//     lensFlareA *= .45;
    
//     Shader flare_shader = ResourceManager::GetShader("flare_shader");
//     flare_shader.enable();
//     glBlendFunc(GL_ONE, GL_ONE);
//     flareTextures->bind(0);
//     glUniform1i(flare_shader.uniform("textures"), 0);

//     mat4 correctScale = scale(mat4(1), vec3(((float) WindowSize::height / WindowSize::width), 1, 1));

//     for (auto &flare : flares)
//     {
//         mat4 mvp = translate(scale(mat4(1), vec3(flare.scale)), vec3(screenSunPos * flare.dist / flare.scale, 1)) * correctScale;

//         if (flare.rotate)
//         {
//             mvp = rotate(
//                 mvp,
//                 (float) -atan2(screenSunPos.x, screenSunPos.y) + mu::PI * -1.25f,
//                 mu::Z
//             );
//         }
	    
//         glUniformMatrix4fv(flare_shader.uniform("mvp"), 1, GL_FALSE, &(mvp)[0][0]);
//         glUniform1i(flare_shader.uniform("layer"), flare.texture);
//         glUniform4f(flare_shader.uniform("flareColor"), flare.color.r, flare.color.g, flare.color.b, flare.color.a * lensFlareA);

//         Mesh::getQuad()->render();
//     }
//     glEnable(GL_DEPTH_TEST);

//     glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
// }

