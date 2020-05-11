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

CameraState PlanetCamera::calculate(float dt)
{
    if (!planet) throw "No planet assigned";

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
        } else {
            dragUpdate();
        }
    }
    else
    {
        accurateDraggingStarted = false;

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

    float altitude = 5. + maxAltitude * (1. - actualZoom);
    float atmosphereTilt = 1.f - clamp(altitude / maxAtmosphere, 0.f, 1.f);

    // Target camera details
    glm::vec3 translateCam = glm::vec3(0, planet->config.radius, 0);

    glm::mat4 transform(1);
    transform = glm::rotate(transform, glm::radians(lon), mu::Y);
    transform = glm::rotate(transform, glm::radians(lat), mu::X);
    transform = glm::translate(transform, translateCam);

    glm::vec3 position = altitude * glm::normalize(glm::vec3(0.f, 1.f, atmosphereTilt));
    glm::vec3 direction = glm::normalize(mu::ZERO_3 - position);
    glm::vec3 right = glm::normalize(glm::cross(direction, -mu::Z));
    glm::vec3 up = glm::normalize(glm::cross(right, direction)); 

    CameraState state;
    state.position = planet->get_position() + glm::vec3(transform * glm::vec4(position, 1));

    state.direction = transform * glm::vec4(direction, 0);
    state.right = transform * glm::vec4(right, 0); 
    state.up = transform * glm::vec4(up, 0);

    return state;

    // camera->rotate(lon, mu::Y);
    // camera->rotate(lat, mu::X);

    // camera->setupAnimation(transform * glm::vec4(position, 1), 
    //     transform * glm::vec4(direction, 0), 
    //     transform * glm::vec4(right, 0), 
    //     transform * glm::vec4(up, 0), 
    //     dt);
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
    float radius = planet->config.radius;
    
    glm::vec3 intersection, normal;
    if (glm::intersectRaySphere(camera->getPosition(), rayDir, planet->get_position(), radius - offset, intersection, normal)) {
        lonLat.x = glm::degrees(std::atan2(intersection.z, intersection.x) + 180.0f);
        lonLat.y = glm::degrees(glm::acos((intersection.y) / radius));
        return true;
    } 
    else return false;
}
