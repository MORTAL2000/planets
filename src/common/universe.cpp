#include "universe.hpp"

#include "utils/math_utils.h"

// #include "utils/generation/planet_generator.hpp"

Universe::Universe(): earth(generator.generate()) {
    // generator.generate(&earth);
}

void Universe::update(float dt) {
    time += dt;
}

Planet * Universe::getPlanet() {
    return &earth;
}

glm::vec3 Universe::calculateSunDirection() {
    glm::vec3 sunDirZoomedOut = glm::vec3(glm::sin(time * .008), 0, glm::cos(time * .008));
    return glm::normalize(sunDirZoomedOut);
}