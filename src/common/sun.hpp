#pragma once

// Standard Headers
#include <iostream>

// Local Headers
#include "orbital_mass.hpp"
#include "geometry/sphere.hpp"
#include "geometry/mesh.hpp"

class Sun: public OrbitalMass, public Renderable {
    public:
        Sphere shape; 
        SharedMesh sunMesh;

        Sun(float radius);

        void upload();
        void render(RenderType type);
};