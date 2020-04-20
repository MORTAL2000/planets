#include "planet_camera.hpp"

// Standard Headers
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Local Headers
#include "utils/window_size.hpp"
#include "utils/input/mouse_input.hpp"


PlanetCameraMovement::PlanetCameraMovement(Camera *cam, Planet *plt)
    : cam(cam), plt(plt), slightlySmallerPlt(plt->sphere.radius - 10.)
{
}

const double DRAG_UPDATE_STEP = 1. / 60.;
const int DRAG_BUTTON = GLFW_MOUSE_BUTTON_LEFT;

void PlanetCameraMovement::update(double deltaTime, Planet *plttt)
{
    auto prevPos = cam->position;
    plt = plttt;
    
    bool startDrag = MouseInput::justPressed(DRAG_BUTTON),
         dragging = MouseInput::pressed(DRAG_BUTTON),
         stoppedDragging = MouseInput::justReleased(DRAG_BUTTON);
    
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
        bool
            up = MouseInput::mouseY < 35,
            down = MouseInput::mouseY > WindowSize::height - 35,
            left = MouseInput::mouseX < 35,
            right = MouseInput::mouseX > WindowSize::width - 35;

        float moveSpeed = 50. - actualZoom * 20.;

        if (up) lat -= deltaTime * moveSpeed;
        else if (down) lat += deltaTime * moveSpeed;

        if (left) lon -= deltaTime * moveSpeed;
        else if (right) lon += deltaTime * moveSpeed;

        if (stoppedDragging) afterDragTimer = .5;

        if (afterDragTimer > 0)
        {
            glm::vec2 d = dragVelocity() * glm::vec2(afterDragTimer * deltaTime * 20.);
            lon += d.x;
            lat += d.y;
            afterDragTimer -= deltaTime;
        }
    }
    lon = fmod(lon, 360.f);
    lat = fmax(0.f, fmin(180.f, lat));

    zoom = fmin(.92 , fmax(0., zoom + MouseInput::yScroll * .08));
    float prevActualZoom = actualZoom;
    actualZoom = actualZoom * (1. - deltaTime * 10.) + zoom * deltaTime * 10.;

    zoomVelocity = abs(prevActualZoom - actualZoom) / deltaTime;

    cam->position = AXIS::Y * glm::vec3(5. + 235 * (1. - actualZoom));
    cam->position.z += actualZoom * 24.5;

    cam->lookAt(AXIS::ZERO_3, -AXIS::Z);

    glm::vec3 translateCam = glm::vec3(0, plt->sphere.radius, 0);

    glm::mat4 transform(1);
    transform = glm::rotate(transform, glm::radians(lon), AXIS::Y);
    transform = glm::rotate(transform, glm::radians(lat), AXIS::X);
    transform = glm::translate(transform, translateCam);

    cam->position = transform * glm::vec4(cam->position, 1);
    cam->direction = transform * glm::vec4(cam->direction, 0);
    cam->right = transform * glm::vec4(cam->right, 0);
    cam->up = transform * glm::vec4(cam->up, 0);

    cam->update();
}

void PlanetCameraMovement::dragUpdate()
{
    bool lessAccurateMethod = false;

    glm::vec3 rayDir = cam->getCursorRayDirection();

    glm::vec2 cursorLonLat;
    if (slightlySmallerPlt.rayToLonLat(cam->position, rayDir, cursorLonLat))
    {
        plt->rayToLonLat(cam->position, rayDir, cursorLonLat);
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

glm::vec2 PlanetCameraMovement::dragVelocity() const
{
    if (dragHistory.size() == 0) return glm::vec2(0);
    return Planet::deltaLonLat(dragHistory.front(), glm::vec2(lon, lat)) / glm::vec2(dragHistory.size());
}


void PlanetCameraMovement::updateHorizonDistance()
{
    horizonDistance = sqrt(pow(length(cam->position), 2) - pow(plt->sphere.radius, 2));
}
