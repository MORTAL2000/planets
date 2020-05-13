#include "universe.hpp"

// Standard Headers
#include <glm/gtx/transform.hpp>
#include <vector>

// Local Headers
#include "scene.hpp"
#include "common/sun.hpp"
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
    config.radius = EARTH_RADIUS / 2.f;
    config.mass = 0.5;

    config.orbit.eccentricity = 0.2;
    // config.color = glm::vec3(0.f, 0.f, 1.f);
    // config.center = glm::vec3(300, 0, 300);

    return config;
}

static PlanetConfig getEarthConfig() {
    PlanetConfig config;
    config.name = "Earth";
    config.radius = EARTH_RADIUS;
    config.mass = 2;
    // config.color = glm::vec3(0.f, 0.f, 1.f);

    return config;
}

Universe::Universe() {
    Sun * sun = new Sun(300);
    sun->set_position(glm::vec3(2000.f, 0, 2000.f));
    sun->upload();
    suns.push_back(sun);

    Planet * earth = new Planet(getEarthConfig());
    earth->set_position(glm::vec3(0.f));
    generator.generate(earth);
    planets.push_back(earth);
    renderables.push_back(earth);

    Planet * earth2 = new Planet(getEarth2Config());
    earth2->set_position(glm::vec3(0.f, 0.f, 600.f));
    generator.generate(earth2);
    planets.push_back(earth2);
    renderables.push_back(earth2);

    center = earth;
    // earth->center = sun;
    earth2->center = earth;

    // earth->a = glm::distance(earth->get_position(), sun->get_position());
    earth2->a = glm::distance(earth2->get_position(), earth->get_position());

//    Spacecraft * spacecraft = new Spacecraft();
//    spacecraft->center = earth;

    generateOrbitalData();
    earth->uploadOrbit();
    earth2->uploadOrbit();
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

    simulationSpeed *= (KeyInput::justPressed(GLFW_KEY_KP_ADD) ? 2 : (KeyInput::justPressed(GLFW_KEY_KP_SUBTRACT) ? .5 : 1));
    simulationDt = simulationSpeed * dt;
    simulationTime += simulationDt;
    

    if (Globals::scene->selected && KeyInput::justPressed(GLFW_KEY_R))
    {
        generator.generate(Globals::scene->selected);
        debugOpen = true;
    }

    for (size_t i = 0; i < planets.size(); i++)
    {
        if (planets[i] == center) continue;

        planets[i]->update(simulationTime);
    }

    if (debugOpen) PlanetGenerator::ShowDebugWindow(&debugOpen);
}

// Planet * Universe::getPlanet() {
//     return static_cast<Planet *>(planets[0]);
// }
const std::vector<Planet*> & Universe::getPlanets() const {
    return planets;
}

const std::vector<Sun*> & Universe::getSuns() const {
    return suns;
}

const std::vector<Renderable*> & Universe::getRenderables() const {
    return renderables;
}

// glm::vec3 Universe::calculateSunDirection(float lat, float lon, float zoom) {

    

//     glm::vec3 sunDirZoomedOut = glm::vec3(glm::sin(time * .008), 0, glm::cos(time * .008));
//     glm::vec3 sunDirZoomedIn = glm::rotate(mu::Z, (lat - float(90.)) * mu::DEGREES_TO_RAD, mu::X);
//     sunDirZoomedIn = glm::rotate(sunDirZoomedIn, (lon + float(20.)) * mu::DEGREES_TO_RAD, mu::Y);
//     float closeSunDir = clamp((zoom - .35) * 3., 0., .7);
//     glm::vec3 sunDir = sunDirZoomedOut * (1 - closeSunDir) + sunDirZoomedIn * closeSunDir;
//     return normalize(sunDir);
// }
