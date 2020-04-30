#include "universe.hpp"

// Standard Headers
#include <glm/gtx/transform.hpp>

// Local Headers
#include "scene.hpp"
#include "utils/math_utils.h"
#include "utils/file/file.h"

// #include "utils/generation/planet_generator.hpp"
#include "utils/generation/earth_generator.h"

const float EARTH_RADIUS = 150, ATMOSPHERE_RADIUS = 198;

static PlanetConfig getEarthConfig() {
    PlanetConfig config;
    config.name = "Earth";
    config.radius = EARTH_RADIUS;
    // config.color = glm::vec3(0.f, 0.f, 1.f);
    config.segments = 100;
    config.rings = 70;

    return config;
}

Universe::Universe(const char *loadFilePath): earth(getEarthConfig()) {
    if (loadFilePath)
    {
        auto earthBin = File::readBinary(loadFilePath);
        earth.fromBinary(earthBin);
    } else {
        generateEarth(&earth);
        std::vector<uint8> data;
        earth.toBinary(data);
        File::writeBinary("level.save", data);
    }
}

void Universe::update(float dt) {
    time += dt;
}

Planet * Universe::getPlanet() {
    return &earth;
}

glm::vec3 Universe::calculateSunDirection(float lat, float lon, float zoom) {
    glm::vec3 sunDirZoomedOut = glm::vec3(glm::sin(time * .008), 0, glm::cos(time * .008));
    glm::vec3 sunDirZoomedIn = glm::rotate(mu::Z, (lat - float(90.)) * mu::DEGREES_TO_RAD, mu::X);
    sunDirZoomedIn = glm::rotate(sunDirZoomedIn, (lon + float(20.)) * mu::DEGREES_TO_RAD, mu::Y);
    float closeSunDir = clamp((zoom - .35) * 3., 0., .7);
    glm::vec3 sunDir = sunDirZoomedOut * (1 - closeSunDir) + sunDirZoomedIn * closeSunDir;
    return normalize(sunDir);
}