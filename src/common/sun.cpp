#include "sun.hpp"

#include "graphics/vert_buffer.hpp"

Sun::Sun(float radius): OrbitalMass({0.f, 0, 0, 0, 0}, 5), shape(radius) {
    std::cout << "Sun created\n";

} 

void Sun::upload() {
    VertAttributes sunAttrs;
    sunAttrs.add_(VertAttributes::POSITION)
        .add_(VertAttributes::NORMAL)
        .add_(VertAttributes::TEX_COORDS)
        .add_(VertAttributes::TANGENT);

    sunMesh = shape.generate("sun", 100, 70, sunAttrs);
    VertBuffer::uploadSingleMesh(sunMesh);
}

void Sun::render(RenderType type) {
    if (type == RenderType::Terrain) sunMesh->render();
}