#pragma once

// Standard Headers
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <queue>

// Local Headers
#include "utils/math_utils.h"

class Camera {
    public:
        Camera(float viewportWidth, float viewportHeight, float fov);
        void ShowDebugWindow(bool* p_open);
        
        glm::vec3 position, direction, up, right;
        glm::mat4 projection, view, combined;
        float viewportWidth, viewportHeight, fov;

        float last_z_near, last_z_far;

        glm::vec3 sunDir;

        void calculate(float z_near, float z_far);
    
        glm::vec3 getCursorRayDirection() const;
        glm::vec3 getRayDirection(float viewportX, float viewportY) const;

        void orientUp(glm::vec3 new_up);
        void lookAt(glm::vec3 target);
        void lookAt(glm::vec3 target, glm::vec3 localYAxis);
        
        void rotate(float degrees, glm::vec3 axis);

            /**
     * returns the point in 'normalized device space'.
     * IF inViewport is originally set to false then inViewport will be set to true if the point is inside the viewport.
     */
    vec3 project(const vec3 &p, bool &inViewport) const;
    vec3 project(const vec3 &p) const;
};