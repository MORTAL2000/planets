#include "flying_camera.hpp"

// Standard Headers
#include <glm/gtx/string_cast.hpp>
#include <iostream>

#include "graphics/window_size.hpp"
#include "graphics/input/key_input.hpp"
#include "graphics/input/mouse_input.hpp"


FlyingCamera::FlyingCamera(Camera *cam)
: camera(cam)
{
    MouseInput::setLockedMode(true);
}

void FlyingCamera::update(float dt)
{
    speedMultiplier += MouseInput::yScroll * .5;
    if (speedMultiplier < 0) speedMultiplier = 0;

    if (KeyInput::pressed(GLFW_KEY_W))
        camera->position += camera->direction * glm::vec3(dt * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_S))
        camera->position += camera->direction * glm::vec3(-dt * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_D))
        camera->position += camera->right * glm::vec3(dt * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_A))
        camera->position += camera->right * glm::vec3(-dt * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_LEFT_SHIFT))
        camera->position.y -= dt * speedMultiplier;

    if (KeyInput::pressed(GLFW_KEY_SPACE))
        camera->position.y += dt * speedMultiplier;

    if (MouseInput::deltaMouseX != 0)
        camera->rotate(MouseInput::deltaMouseX / WindowSize::width * -100 * mouseSensivity, camera->up);

    if (MouseInput::deltaMouseY != 0)
        camera->rotate(MouseInput::deltaMouseY / WindowSize::height * -100 * mouseSensivity, camera->right);
    
    // std::cout << glm::to_string(camera->position) << std::endl;
    camera->update();
}
