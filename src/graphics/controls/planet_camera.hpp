#pragma once

// Standard Headers
#include <queue>

// Local Headers
#include "graphics/camera.hpp"
#include "common/planet.hpp"

class PlanetCamera
{
  public:

    PlanetCamera(Camera *cam);


    void update(float dt);

    float speedMultiplier = 1;
    float zoomVelocity = 0;
    float lon = 160, lat = 75, zoom = 0, actualZoom = 0;
    float horizonDistance = -1;

  private:
    void dragUpdate();
    glm::vec2 dragVelocity() const;
    
    bool cursorToLonLat(const glm::vec3 & rayDir, vec2 &lonLat, float offset) const;

    float radius = 150;
    float maxAltitude = 1000;
    Camera *camera;

    float dragLon = 0, dragLat = 0, afterDragTimer;
    bool accurateDraggingStarted = false;

    std::queue<glm::vec2> dragHistory;
};


