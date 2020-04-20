#include "universe.hpp"

Universe::Universe(): earth(EARTH_RADIUS) {}

void Universe::update(float dt) {
    time += dt;
}