
#include "cloud_renderer.hpp"

// Local Headers
#include "graphics/vert_buffer.hpp"
#include "utils/math_utils.h"
#include "utils/math/interpolation.h"
#include "scene.hpp"

const int nrOfSpawnpoints = 30, particlesPerOffset = 6;

CloudRenderer::CloudRenderer():   
    quad(new Mesh("cloud_quad", 4, 6, Mesh::getQuad()->attributes))
{
    ResourceManager::LoadShader("clouds.vert", "clouds.frag", "clouds");
    noiseTex = ResourceManager::LoadTexture("textures/noise.dds", "noise");

    quad->vertices = Mesh::getQuad()->vertices;
    quad->indices = Mesh::getQuad()->indices;
    VertBuffer::uploadSingleMesh(quad);

    VertAttributes verts;
    verts.add({"SPAWNPOINT", 3});

    VertData spawnpoints(verts, std::vector<u_char>(nrOfSpawnpoints * 4 * 3));
    for (int i = 0; i < nrOfSpawnpoints; i++)
        spawnpoints.set(
                vec3(mu::random() - .5, (mu::random() - .5) * .3, mu::random() - .5)
                * vec3(1 + i * .2) * vec3(110.),
                i, 0
        );

    quad->vertBuffer->uploadPerInstanceData(spawnpoints, particlesPerOffset);
}

void CloudRenderer::render(double dt)
{
    Camera camera = Globals::scene->getCamera();
    Universe universe = Globals::scene->getUniverse();

    float planetRadius = 150;

    if (clouds.size() == 0) dt = 30;
    while (clouds.size() < 40)
        clouds.push_back({
            mu::random(360), // lon
            mu::random(mu::random() > .5 ? 40. : 0., mu::random() > .5 ? 120. : 180.), // lat
            mu::random(.7, 1.3), // speed
            0,
            mu::random(30, 60), // time to live
            mu::randomInt(3, 30) // nr of particles
        });

    Shader shader = ResourceManager::GetShader("clouds");
    shader.enable();

    glUniform1f(shader.uniform("time"), universe.getTime());
   
    noiseTex->bind(0);
    glUniform1i(shader.uniform("noiseTex"), 0);

    glDepthMask(false);

    for (int i = clouds.size() - 1; i >= 0; i--)
    {
        auto &cloud = clouds[i];

        cloud.timeSinceSpawn += dt;
        cloud.timeToDespawn -= dt;

        if (cloud.timeToDespawn <= 0) clouds.erase(clouds.begin() + i);

        cloud.lon += .7 * dt * cloud.speed;
        cloud.lat += .4 * dt * cloud.speed;

        mat4 t = rotate(mat4(1.), cloud.lon * mu::DEGREES_TO_RAD, mu::Y);
        t = rotate(t, cloud.lat * mu::DEGREES_TO_RAD, mu::X);
        t = translate(t, vec3(0, planetRadius + 35, 0));

        CameraState state = camera.getState();
        glm::vec3 up = inverse(t) * vec4(state.up, 0);
        glm::vec3 right = inverse(t) * vec4(state.right, 0);
        t = camera.combined * t;

        float light = 0;
        light += max(0.f, 1 - Interpolation::circleIn(min(1., cloud.lat / 30.)));
        light += max(0.f, 1 - Interpolation::circleIn(min(1., (180 - cloud.lat) / 30.)));

        light += min(1.f, max(0.0f, dot(rotate(vec3(0, 0, 1), cloud.lon * mu::DEGREES_TO_RAD, mu::Y), camera.sunDir) + .8f));

        glUniformMatrix4fv(shader.uniform("mvp"), 1, GL_FALSE, &t[0][0]);
        glUniform3f(shader.uniform("up"), up.x, up.y, up.z);
        glUniform3f(shader.uniform("right"), right.x, right.y, right.z);
        glUniform1f(shader.uniform("cloudOpacity"), Interpolation::circleIn(max(0.f, min(cloud.timeToDespawn, cloud.timeSinceSpawn)) / 20.));
        glUniform1f(shader.uniform("light"), light);
        quad->renderInstances(cloud.spawnPoints * particlesPerOffset);
    }
    glDepthMask(true);
}
