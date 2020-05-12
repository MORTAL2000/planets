#pragma once

// Standard Headers
#include <iostream>
#include <vector>
#include <optional>
#include <glm/vec3.hpp>

// Local Headers
#include "graphics/renderable.hpp"
#include "common/maneuver.hpp"
#include "common/orbital_mass.hpp"

// struct Mission
// {
//     std::vector<Maneuver> maneuvers;
//     float progress = 0.;
// };

class Spacecraft: public Renderable {
//    glm::vec3 relative_velocity;

    // std::optional<Mission> mission;
    public:
        Spacecraft();

        void upload();
        void render(RenderType type);
};
