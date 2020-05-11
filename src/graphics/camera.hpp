#pragma once

// Standard Headers
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <queue>

// Local Headers
#include "utils/math_utils.h"

static float MAX_SPEED = 500; // 100 unit per second
static float MAX_ROTATION = 30; // 30 degrees per second

struct CameraState {
    glm::vec3 position, direction, up, right;
};

class Camera {
    private:

        float posT = 0, posTotalTime = 0;
        // float rotT = 0, rotTotalTime = 0;
        glm::vec3 targetPosition, lastPosition;
        glm::quat targetRotation, lastRotation;

        // void setupAnimation(glm::vec3 pos, glm::vec3 dir, glm::vec3 up, glm::vec3 right, float time);
        // void updateAnimation(float dt);
        CameraState state;
    public:
        Camera(float viewportWidth, float viewportHeight, float fov);
        void ShowDebugWindow(bool* p_open);
        
        glm::mat4 projection, view, combined;
        float viewportWidth, viewportHeight, fov;

        float last_z_near, last_z_far;

        glm::vec3 sunDir;

        void calculate(float z_near, float z_far, float dt = 0.f);
    
        glm::vec3 getCursorRayDirection() const;
        glm::vec3 getRayDirection(float viewportX, float viewportY) const;

        const CameraState & getState() const { return state; }
        const glm::vec3 & getPosition() const { return state.position; }

        void orientUp(glm::vec3 new_up);
        void lookAt(glm::vec3 target);
        void lookAt(glm::vec3 target, glm::vec3 localYAxis);
        
        void rotate(float degrees, glm::vec3 axis);
        void moveTo(CameraState newState, bool calculateAnimation = false);
        void rotateTo(glm::vec3 direction, glm::vec3 right, glm::vec3 up);

            /**
     * returns the point in 'normalized device space'.
     * IF inViewport is originally set to false then inViewport will be set to true if the point is inside the viewport.
     */
    vec3 project(const vec3 &p, bool &inViewport) const;
    vec3 project(const vec3 &p) const;
};