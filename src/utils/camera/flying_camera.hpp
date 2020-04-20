
#pragma once

// Local Headers
#include "rendering/camera.hpp"

class FlyingCamera
{

  public:

    Camera *cam;

    float speedMultiplier = 1, mouseSensivity = 1;

    FlyingCamera(Camera *cam);

    void update(double deltaTime);

};
