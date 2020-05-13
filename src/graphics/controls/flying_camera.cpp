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

CameraState FlyingCamera::calculate(float dt)
{
    speedMultiplier += MouseInput::yScroll * .5;
    if (speedMultiplier < 0) speedMultiplier = 0;

    CameraState state = camera->getState();

    if (KeyInput::pressed(GLFW_KEY_O))
        camera->orientUp(glm::normalize(state.position));
    
    if (KeyInput::pressed(GLFW_KEY_W))
        state.position += state.direction * glm::vec3(dt * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_S))
        state.position += state.direction * glm::vec3(-dt * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_D))
        state.position += state.right * glm::vec3(dt * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_A))
        state.position += state.right * glm::vec3(-dt * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_Q))
        camera->rotate(-dt * speedMultiplier, state.direction, state);

    if (KeyInput::pressed(GLFW_KEY_E))
        camera->rotate(dt * speedMultiplier, state.direction, state);

    if (KeyInput::pressed(GLFW_KEY_LEFT_SHIFT))
        state.position -= state.up * glm::vec3(dt * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_SPACE))
        state.position += state.up * glm::vec3(dt * speedMultiplier);

    if (MouseInput::deltaMouseX != 0)
        camera->rotate(MouseInput::deltaMouseX / WindowSize::width * -100 * mouseSensivity, state.up, state);

    if (MouseInput::deltaMouseY != 0)
        camera->rotate(MouseInput::deltaMouseY / WindowSize::height * -100 * mouseSensivity, state.right, state);
    
    // std::cout << glm::to_string(camera->position) << std::endl;
    // camera->update();
    return state;
}

