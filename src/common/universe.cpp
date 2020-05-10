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

static PlanetConfig getEarthConfig() {
    PlanetConfig config;
    config.name = "Earth";
    config.radius = EARTH_RADIUS;
    // config.color = glm::vec3(0.f, 0.f, 1.f);

    return config;
}

Universe::Universe() {
    // center = new OrbitalMass();

    Planet * earth = new Planet(getEarthConfig());
    center = earth;

    Spacecraft * spacecraft = new Spacecraft();
    spacecraft->center = earth;

    generator.generate(earth);
    planets.push_back(earth);

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
        generator.generate(getPlanet());
        debugOpen = true;
    }

    if (debugOpen) PlanetGenerator::ShowDebugWindow(&debugOpen);
}

Planet * Universe::getPlanet() {
    return static_cast<Planet *>(planets[0]);
}

glm::vec3 Universe::calculateSunDirection(float lat, float lon, float zoom) {
    glm::vec3 sunDirZoomedOut = glm::vec3(glm::sin(time * .008), 0, glm::cos(time * .008));
    glm::vec3 sunDirZoomedIn = glm::rotate(mu::Z, (lat - float(90.)) * mu::DEGREES_TO_RAD, mu::X);
    sunDirZoomedIn = glm::rotate(sunDirZoomedIn, (lon + float(20.)) * mu::DEGREES_TO_RAD, mu::Y);
    float closeSunDir = clamp((zoom - .35) * 3., 0., .7);
    glm::vec3 sunDir = sunDirZoomedOut * (1 - closeSunDir) + sunDirZoomedIn * closeSunDir;
    return normalize(sunDir);
}
