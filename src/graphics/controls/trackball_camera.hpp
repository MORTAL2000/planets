#pragma once

// Standard Headers
#include <queue>
#include <limits>

// Local Headers
#include "graphics/camera.hpp"
#include "common/planet.hpp"

class PlanetCamera
{
  public:

    PlanetCamera(Camera *cam, Planet *plt);

    void update(float dt);

    float m_rotateSpeed;
		float m_zoomSpeed;

		float m_dynamicDampingFactor;
		float m_minDistance;
		float m_maxDistance;

		bool m_noRoll;
		bool m_staticMoving;

    // float horizonDistance = -1;

  private:
    // void dragUpdate();
    // glm::vec2 dragVelocity() const;

    void planetCulling();

    void updateHorizonDistance();
    // bool cursorToLonLat(const glm::vec3 & rayDir, vec2 &lonLat, float offset) const;

    Planet *plt;
    Camera *camera;

    glm::vec3 m_target;
		glm::vec3 m_lastPos;
		glm::vec3 m_eye;

		glm::vec3 m_movePrev;
		glm::vec3 m_moveCurr;

    glm::vec3 m_lastAxis;
    float m_lastAngle;

    void RotateCamera();
    // void CheckDistances();


    float lastZoom = 1;

    // bool draggingStarted = false;

    // float dragUpdateAccumulator = 0, afterDragTimer;
    // bool accurateDraggingStarted = false;

    // std::queue<glm::vec2> dragHistory;
    glm::vec2 getMouseOnCircle(int clientX, int clientY);
};


