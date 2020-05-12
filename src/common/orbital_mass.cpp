#include "orbital_mass.hpp"

#include "utils/orbital_utils.h"

OrbitalMass::OrbitalMass(OrbitalParameters config, float mass):
    e(config.eccentricity),
    i(config.inclination),
    loPE(config.longitudePeriapsis),
    loAN(config.longitudeAscendingNode),
    rL(config.trueAnomaly),
    mass(mass) {


    // a = glm::distance()
    gravParam = mass * ou::GRAVITATIONAL_CONSTANT;

    // axialTilt = glm::vec3(0.f, 1.f, 0.f);
    orbitalAxis = ou::calculateAxialTilt(this);

    // Potentially correct argument of periapsis to longitude of periapsis
}

void OrbitalMass::calculateSOI() {
    // Calculate the sphere of influence
    SOI = a * pow(gravParam / (3.f * center->gravParam), 1.f / 3.f);
}
