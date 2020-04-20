#pragma once

// Standard Headers
#include <queue>

// Local Headers
#include "rendering/camera.hpp"
#include "common/planet.hpp"

class PlanetCameraMovement
{
  public:

    PlanetCameraMovement(Camera *cam, Planet *plt);

    void update(double deltaTime, Planet *plt);

    float zoomVelocity = 0;
    float lon = 160, lat = 75, zoom = 0, actualZoom = 0;
    float horizonDistance = -1;

  private:
    void dragUpdate();
    glm::vec2 dragVelocity() const;

    void islandFrustumCulling();

    void updateHorizonDistance();

    Camera *cam;
    Planet *plt, slightlySmallerPlt; // slightlySmallerPlt is just a dummy planet with a smaller radius used in dragUpdate()

    float dragLon = 0, dragLat = 0, dragUpdateAccumulator = 0, afterDragTimer;
    bool accurateDraggingStarted = false;

    std::queue<glm::vec2> dragHistory;
};


