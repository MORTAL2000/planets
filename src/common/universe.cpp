#include "universe.hpp"

// Standard Headers
#include <glm/gtx/transform.hpp>
#include <vector>

// Local Headers
#include "scene.hpp"
#include "entities/space_craft.hpp"
#include "utils/math_utils.h"
#include "utils/orbital_utils.h"
#include "utils/file/file.h"

#include "utils/generation/planet_generator.hpp"
#include "graphics/input/key_input.hpp"

const float EARTH_RADIUS = 150; //ATMOSPHERE_RADIUS = 198;

static PlanetConfig getEarth2Config() {
    PlanetConfig config;
    config.name = "Earth2";
    config.radius = EARTH_RADIUS;
    // config.color = glm::vec3(0.f, 0.f, 1.f);
    // config.center = glm::vec3(300, 0, 300);

    return config;
}

static PlanetConfig getEarthConfig() {
    PlanetConfig config;
    config.name = "Earth";
    config.radius = EARTH_RADIUS;
    // config.color = glm::vec3(0.f, 0.f, 1.f);

    return config;
}

Universe::Universe() {
    Planet * earth = new Planet(getEarthConfig());
    earth->set_position(glm::vec3(0.f));
    generator.generate(earth);
    planets.push_back(earth);
    renderables.push_back(earth);

    Planet * earth2 = new Planet(getEarth2Config());
    earth2->set_position(glm::vec3(300.f, 0.f, 300.f));
    generator.generate(earth2);
    planets.push_back(earth2);
    renderables.push_back(earth2);

    center = earth;
    Spacecraft * spacecraft = new Spacecraft();
    spacecraft->center = earth;

    generateOrbitalData();
}


void Universe::generateOrbitalData() {
    for (size_t i = 0; i < planets.size(); i++)
    {
        if (planets[i] == center) continue;

        ou::generateOrbitalCoords(planets[i]);
        ou::generateOrbitalTimes(planets[i]);
    }
}

void Universe::update(float dt) {
    time += dt;

    if (KeyInput::justPressed(GLFW_KEY_R))
    {
        // generator.generate(getPlanet());
        debugOpen = true;
    }

    if (debugOpen) PlanetGenerator::ShowDebugWindow(&debugOpen);
}

// Planet * Universe::getPlanet() {
//     return static_cast<Planet *>(planets[0]);
// }
std::vector<Planet*> Universe::getPlanets() {
    return planets;
}

std::vector<Renderable*> Universe::getRenderables() {
    return renderables;
}

glm::vec3 Universe::calculateSunDirection(float lat, float lon, float zoom) {
    glm::vec3 sunDirZoomedOut = glm::vec3(glm::sin(time * .008), 0, glm::cos(time * .008));
    glm::vec3 sunDirZoomedIn = glm::rotate(mu::Z, (lat - float(90.)) * mu::DEGREES_TO_RAD, mu::X);
    sunDirZoomedIn = glm::rotate(sunDirZoomedIn, (lon + float(20.)) * mu::DEGREES_TO_RAD, mu::Y);
    float closeSunDir = clamp((zoom - .35) * 3., 0., .7);
    glm::vec3 sunDir = sunDirZoomedOut * (1 - closeSunDir) + sunDirZoomedIn * closeSunDir;
    return normalize(sunDir);
}
