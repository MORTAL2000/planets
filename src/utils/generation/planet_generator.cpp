#include "planet_generator.hpp"

PlanetGenerator::PlanetGenerator() {}

PlanetConfig PlanetGenerator::generate() {
    PlanetConfig config;
    config.name = "Earth";
    config.radius = EARTH_RADIUS;
    // config.color = glm::vec3(0.f, 0.f, 1.f);
    config.segments = 100;
    config.rings = 70;

    return config;
}