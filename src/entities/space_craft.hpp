#pragma once

// Standard Headers
#include <iostream>
#include <vector>
#include <optional>
#include <glm/vec3.hpp>

#include "common/maneuver.hpp"
#include "common/orbital_mass.hpp"

// struct Mission
// {
//     std::vector<Maneuver> maneuvers;
//     float progress = 0.;
// };

class Spacecraft: public OrbitalMass {
//    glm::vec3 relative_velocity;

    // std::optional<Mission> mission;
};
