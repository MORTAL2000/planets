#pragma once

// Standard Headers
#include <glm/glm.hpp>

// Local Headers
#include "geometry/mesh.hpp"


namespace TangentCalculator
{

/**
 * Returns the tangent of triangle (p0, p1, p2).
 * For each point in the triangle an UV point (texture coords) is required.
 * https://youtu.be/4DUfwAEx4Ts?t=423
 */
glm::vec3 calculateTangent(
    glm::vec3 &p0, glm::vec3 &p1, glm::vec3 &p2,
    glm::vec2 &uv0, glm::vec2 &uv1, glm::vec2 &uv2);

/**
 * Adds Tangents to mesh.
 * Mesh must have attributes including: VertAttributes::POSITION, VertAttributes::TEX_COORDS, VertAttributes::TANGENT.
 */
void addTangentsToMesh(SharedMesh mesh);
void addTangentsToMesh(Mesh *mesh);

} // namespace TangentCalculator