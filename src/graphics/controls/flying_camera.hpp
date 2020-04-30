
#pragma once

// Local Headers
#include "graphics/camera.hpp"

class FlyingCamera
{

  public:

    Camera *camera;

    float speedMultiplier = 1, mouseSensivity = 1;

    FlyingCamera(Camera *cam);

    void update(float dt);

};
