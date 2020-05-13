
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
    unsigned int spawnpoint_offset = verts.add({"SPAWNPOINT", 3});

    const Universe & universe = Globals::scene->getUniverse();
    const std::vector<Planet * > & planets = universe.getPlanets();

    VertData spawnpoints(verts, std::vector<u_char>(nrOfSpawnpoints * 4 * 3 * planets.size()));
    // for (int p = 0; p < planets.size(); p++) {
        int p = 0;
        for (int i = 0; i < nrOfSpawnpoints; i++) {
            glm::vec3 spawn = glm::vec3(mu::random() - .5, (mu::random() - .5) * .3, mu::random() - .5) * vec3(1 + i * .2) * vec3(110.);
            // spawn += planets[p]->get_position();
            spawnpoints.set(spawn, p * nrOfSpawnpoints + i, spawnpoint_offset);
        }
    // }

    quad->vertBuffer->uploadPerInstanceData(spawnpoints, particlesPerOffset);
}

void CloudRenderer::render(double realtimeDT) {
    const Universe & universe = Globals::scene->getUniverse();
    const std::vector<Planet * > & planets = universe.getPlanets();

    for (Planet * planet : universe.getPlanets()) {
        // The dt above is real time. The universe dt is the simulations. 
        render(planet, universe.getTime(), universe.getDeltaTime());
    }
}

void CloudRenderer::render(Planet * planet, double time, float deltaTime)
{
    float planetRadius = 150;

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

    glUniform1f(shader.uniform("time"), time);
   
    noiseTex->bind(0);
    glUniform1i(shader.uniform("noiseTex"), 0);

    glDepthMask(false);

    for (int i = clouds.size() - 1; i >= 0; i--)
    {
        auto &cloud = clouds[i];

        cloud.timeSinceSpawn += deltaTime;
        cloud.timeToDespawn -= deltaTime;

        if (cloud.timeToDespawn <= 0) clouds.erase(clouds.begin() + i);

        cloud.lon += .7 * deltaTime * cloud.speed;
        cloud.lat += .4 * deltaTime * cloud.speed;
        
        glm::mat4 transformFromCenter = rotate(glm::mat4(1.f), glm::radians(cloud.lon), mu::Y);
        transformFromCenter = rotate(transformFromCenter, glm::radians(cloud.lat), mu::X);
        transformFromCenter = translate(transformFromCenter, vec3(0, planet->config.radius + planet->config.cloudHeight, 0));

        glm::mat4 transformFromCenterInverse = glm::inverse(transformFromCenter);

        CameraState state = camera->getState();
        glm::vec3 up = transformFromCenterInverse * vec4(state.up, 0);
        glm::vec3 right = transformFromCenterInverse * vec4(state.right, 0);
 
        glm::mat4 transform = camera->combined * planet->get_last_model() * transformFromCenter;

        float light = 0;
        light += max(0.f, 1 - Interpolation::circleIn(min(1., cloud.lat / 30.)));
        light += max(0.f, 1 - Interpolation::circleIn(min(1., (180 - cloud.lat) / 30.)));

        light += min(1.f, max(0.0f, dot(rotate(vec3(0, 0, 1), cloud.lon * mu::DEGREES_TO_RAD, mu::Y), camera->sunDir) + .8f));

        glUniformMatrix4fv(shader.uniform("mvp"), 1, GL_FALSE, glm::value_ptr(transform));
        glUniform3f(shader.uniform("up"), up.x, up.y, up.z);
        glUniform3f(shader.uniform("right"), right.x, right.y, right.z);
        glUniform1f(shader.uniform("cloudOpacity"), Interpolation::circleIn(max(0.f, min(cloud.timeToDespawn, cloud.timeSinceSpawn)) / 20.));
        glUniform1f(shader.uniform("light"), light);
        quad->renderInstances(cloud.spawnPoints * particlesPerOffset);
    }
    glDepthMask(true);
}
