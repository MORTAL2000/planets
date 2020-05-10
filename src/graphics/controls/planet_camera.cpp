#include "planet_camera.hpp"

// Standard Headers
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/intersect.hpp>

// Local Headers
#include "graphics/window_size.hpp"
#include "graphics/input/key_input.hpp"
#include "graphics/input/mouse_input.hpp"
#include "graphics/imgui/imgui.h"


PlanetCamera::PlanetCamera(Camera *cam)
    : camera(cam)
{
}

const int DRAG_BUTTON = GLFW_MOUSE_BUTTON_LEFT;

void PlanetCamera::update(float dt)
{
    if (KeyInput::pressed(GLFW_KEY_W))
        camera->position += camera->direction * glm::vec3(dt * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_S))
        camera->position += camera->direction * glm::vec3(-dt * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_D))
        camera->position += camera->right * glm::vec3(dt * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_A))
        camera->position += camera->right * glm::vec3(-dt * speedMultiplier);

    if (KeyInput::pressed(GLFW_KEY_Q))
        camera->rotate(-dt * speedMultiplier, camera->direction);

    if (KeyInput::pressed(GLFW_KEY_E))
        camera->rotate(dt * speedMultiplier, camera->direction);
    
    bool startDrag = MouseInput::justPressed(DRAG_BUTTON),
         dragging = MouseInput::pressed(DRAG_BUTTON),
         stoppedDragging = MouseInput::justReleased(DRAG_BUTTON);

    if (MouseInput::mouseX < 0 || MouseInput::mouseY < 0 || MouseInput::mouseX > WindowSize::width || MouseInput::mouseY > WindowSize::height) {
        startDrag  = false;
        dragging = false;
        stoppedDragging = false;
    }
    
    if (dragging || startDrag)
    {
        if (startDrag) {
            std::queue<glm::vec2> empty;
            std::swap(dragHistory, empty);
        }
        
        dragUpdate();
    }
    else
    {
        accurateDraggingStarted = false;
        // bool
        //     up = MouseInput::mouseY < 35,
        //     down = MouseInput::mouseY > WindowSize::height - 35,
        //     left = MouseInput::mouseX < 35,
        //     right = MouseInput::mouseX > WindowSize::width - 35;

        // float moveSpeed = 50. - actualZoom * 20.;

        // if (up) lat -= dt * moveSpeed;
        // else if (down) lat += dt * moveSpeed;

        // if (left) lon -= dt * moveSpeed;
        // else if (right) lon += dt * moveSpeed;

        // Slowly drift to an orbit after a timeout

        if (stoppedDragging) afterDragTimer = 1;

        if (afterDragTimer > 0)
        {
            glm::vec2 d = dragVelocity() * glm::vec2(afterDragTimer * dt * 20.);
            lon += d.x;
            lat += d.y;
            afterDragTimer -= dt;
        }
    }
    lon = fmod(lon, 360.f);
    lat = fmax(0.f, fmin(180.f, lat));
 
    zoom = fmin(.92 , fmax(0., zoom + MouseInput::yScroll * .08));
    float prevActualZoom = actualZoom;
    actualZoom = actualZoom * (1. - dt * 10.) + zoom * dt * 10.;

    zoomVelocity = abs(prevActualZoom - actualZoom) / dt;

    camera->position = mu::Y * glm::vec3(5. + maxAltitude * (1. - actualZoom));
    camera->position.z += actualZoom * 24.5;

    camera->lookAt(mu::ZERO_3, -mu::Z);

    glm::vec3 translateCam = glm::vec3(0, radius, 0);

    glm::mat4 transform(1);
    transform = glm::rotate(transform, glm::radians(lon), mu::Y);
    transform = glm::rotate(transform, glm::radians(lat), mu::X);
    transform = glm::translate(transform, translateCam);

    camera->position = transform * glm::vec4(camera->position, 1);
    camera->direction = transform * glm::vec4(camera->direction, 0);
    camera->right = transform * glm::vec4(camera->right, 0);
    camera->up = transform * glm::vec4(camera->up, 0);
}

void PlanetCamera::dragUpdate()
{
    bool lessAccurateMethod = false;

    glm::vec3 rayDir = camera->getCursorRayDirection();

    glm::vec2 cursorLonLat;

    // This planet ray test should be slightly smaller, for some reason
    if (cursorToLonLat(rayDir, cursorLonLat, 10.f))
    {
        cursorToLonLat(rayDir, cursorLonLat, 0.f);
        if (!accurateDraggingStarted)
        {
            dragLon = cursorLonLat.x;
            dragLat = cursorLonLat.y;
            accurateDraggingStarted = true;
        }
        float actualLon = fmod(270 - cursorLonLat.x, 360.f);
        float lonDiff = fabs(actualLon - lon);
        lonDiff = fmin(lonDiff, 360 - lonDiff);
        if (lonDiff < 90 && cursorLonLat.y > 15 && cursorLonLat.y < 165)
        {
            float deltaLon = cursorLonLat.x - dragLon,
                deltaLat = cursorLonLat.y - dragLat;

            lon += deltaLon;
            lat -= deltaLat;

            dragLon = cursorLonLat.x - deltaLon;
            dragLat = cursorLonLat.y - deltaLat;

        }
        else lessAccurateMethod = true;
    }
    else lessAccurateMethod = true;

    if (lessAccurateMethod)
    {
        accurateDraggingStarted = false;
        lon -= MouseInput::deltaMouseX * .13;
        lat -= MouseInput::deltaMouseY * .13;
    }
    dragHistory.push(glm::vec2(lon, lat));
    if (dragHistory.size() > 10)
        dragHistory.pop();
}

glm::vec2 PlanetCamera::dragVelocity() const
{
    if (dragHistory.size() == 0) return glm::vec2(0);
    return Planet::deltaLonLat(dragHistory.front(), glm::vec2(lon, lat)) / glm::vec2(dragHistory.size());
}


bool PlanetCamera::cursorToLonLat(const glm::vec3 & rayDir, vec2 &lonLat, float offset) const
{
    glm::vec3 intersection, normal;
    if (glm::intersectRaySphere(camera->position, rayDir, glm::vec3(0.f), radius - offset, intersection, normal)) {
        lonLat.x = glm::degrees(std::atan2(intersection.z, intersection.x) + 180.0f);
        lonLat.y = glm::degrees(glm::acos((intersection.y) / radius));
        return true;
    } 
    else return false;
}
