#pragma once

// Standard Headers
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <queue>

// Local Headers
#include "utils/math_utils.h"

// namespace AXIS
// {
//     const glm::vec3
//         X = glm::vec3(1, 0, 0),
//         Y = glm::vec3(0, 1, 0),
//         Z = glm::vec3(0, 0, 1),
//         ZERO_3 = glm::vec3(0);
// }

class Camera {
    public:
        Camera(float z_near, float z_far, float viewportWidth, float viewportHeight, float fov);
        
        glm::vec3 position, direction, up, right;
        glm::mat4 projection, view, combined;
        float z_near, z_far, viewportWidth, viewportHeight, fov;

        void update();
    
        glm::vec3 getCursorRayDirection() const;
        glm::vec3 getRayDirection(float viewportX, float viewportY) const;

        void lookAt(glm::vec3 target);
        void lookAt(glm::vec3 target, glm::vec3 localYAxis);
        
        void rotate(float degrees, glm::vec3 axis);
};