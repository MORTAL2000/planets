#include "camera.hpp"

// Standard Headers
#include <utility>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Local Headers
#include "common/planet.hpp"
#include "utils/window_size.hpp"
#include "utils/input/key_input.hpp"
#include "utils/input/mouse_input.hpp"

const int DRAG_BUTTON = GLFW_MOUSE_BUTTON_LEFT;



Camera::Camera(float z_near, float z_far, float viewportWidth, float viewportHeight, float fov):
    z_near(z_near), z_far(z_far), viewportWidth(viewportWidth), viewportHeight(viewportHeight), fov(fov),
    direction(glm::vec3(0, 0, -1)),
    up(glm::vec3(0, 1, 0)),
    right(glm::vec3(1, 0, 0))
{
}

glm::vec3 Camera::getRayDirection(float viewportX, float viewportY) const
{
    glm::vec2 point = glm::vec2(viewportX / viewportWidth, 1 - viewportY / viewportHeight) * glm::vec2(2) - glm::vec2(1);

    glm::vec4 eyeCoords = inverse(projection) * glm::vec4(point.x, point.y, -1, 1);
    eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, eyeCoords.z, 0);
    glm::vec3 worldCoords = inverse(view) * eyeCoords;

    return worldCoords;
}

glm::vec3 Camera::getCursorRayDirection() const
{
    return getRayDirection(MouseInput::mouseX, MouseInput::mouseY);
}

void Camera::update()
{
    projection = glm::perspective(glm::radians(fov), viewportWidth / viewportHeight, z_near, z_far);
    view = glm::lookAt(
        position,
        position + direction,
        up
    );
    combined = projection * view;
}

void Camera::lookAt(glm::vec3 target, glm::vec3 localYAxis)
{
    direction = glm::normalize(target - position);
    right = glm::normalize(glm::cross(direction, localYAxis));
    up = glm::normalize(glm::cross(right, direction));
}

void Camera::rotate(float degrees, glm::vec3 axis)
{
    float rads =  glm::radians(degrees);
    direction = glm::rotate(direction, rads, axis);
    right = glm::rotate(right, rads, axis);
    up = glm::rotate(up, rads, axis);
}