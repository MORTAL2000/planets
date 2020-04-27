#include "flying_camera.hpp"

// Standard Headers
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "graphics/window_size.hpp"
#include "graphics/input/key_input.hpp"
#include "graphics/input/mouse_input.hpp"


FlyingCamera::FlyingCamera(Camera *cam)
: cam(cam)
{
    MouseInput::setLockedMode(true);
}

void FlyingCamera::update(double deltaTime)
{
    speedMultiplier += MouseInput::yScroll * .5;
    if (speedMultiplier < 0) speedMultiplier = 0;

    if (KeyInput::pressed(GLFW_KEY_W))
        cam->position += cam->direction * glm::vec3(deltaTime * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_S))
        cam->position += cam->direction * glm::vec3(-deltaTime * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_D))
        cam->position += cam->right * glm::vec3(deltaTime * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_A))
        cam->position += cam->right * glm::vec3(-deltaTime * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_LEFT_SHIFT))
        cam->position.y -= deltaTime * speedMultiplier;

    if (KeyInput::pressed(GLFW_KEY_SPACE))
        cam->position.y += deltaTime * speedMultiplier;

    if (MouseInput::deltaMouseX != 0)
        cam->rotate(MouseInput::deltaMouseX / WindowSize::width * -100 * mouseSensivity, cam->up);

    if (MouseInput::deltaMouseY != 0)
        cam->rotate(MouseInput::deltaMouseY / WindowSize::height * -100 * mouseSensivity, cam->right);
    
    // std::cout << glm::to_string(cam->position) << std::endl;
    cam->update();
}

