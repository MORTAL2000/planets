#include "planet_camera.hpp"

// Standard Headers
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/intersect.hpp>

// Local Headers
#include "graphics/window_size.hpp"
#include "graphics/input/mouse_input.hpp"


PlanetCamera::PlanetCamera(Camera *cam, Planet *plt)
    : camera(cam), plt(plt)
{
}

const int DRAG_BUTTON = GLFW_MOUSE_BUTTON_LEFT;

void PlanetCamera::update(float dt)
{
    auto prevPos = camera->position;
    
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

        if (up) lat -= dt * moveSpeed;
        else if (down) lat += dt * moveSpeed;

        if (left) lon -= dt * moveSpeed;
        else if (right) lon += dt * moveSpeed;

        if (stoppedDragging) afterDragTimer = .5;

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

    camera->position = mu::Y * glm::vec3(5. + 235 * (1. - actualZoom));
    camera->position.z += actualZoom * 24.5;

    camera->lookAt(mu::ZERO_3, -mu::Z);

    glm::vec3 translateCam = glm::vec3(0, plt->sphere.radius, 0);

    glm::mat4 transform(1);
    transform = glm::rotate(transform, glm::radians(lon), mu::Y);
    transform = glm::rotate(transform, glm::radians(lat), mu::X);
    transform = glm::translate(transform, translateCam);

    camera->position = transform * glm::vec4(camera->position, 1);
    camera->direction = transform * glm::vec4(camera->direction, 0);
    camera->right = transform * glm::vec4(camera->right, 0);
    camera->up = transform * glm::vec4(camera->up, 0);

    camera->update();
    if (prevPos != camera->position) planetCulling();
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


void PlanetCamera::updateHorizonDistance()
{
    horizonDistance = sqrt(pow(length(camera->position), 2) - pow(plt->sphere.radius, 2));
}


void PlanetCamera::planetCulling()
{
    updateHorizonDistance();
    int nrInView = 0;
    // for (Island *isl : plt->islands)
    // {
    //     isl->isInView = false;
    //     for (int x = 0; x < isl->width; x += 10)
    //     {
    //         for (int y = 0; y < isl->height && !isl->isInView; y += 10)
    //         {
    //             if (isl->tileAtSeaFloor(x, y)) continue;

    //             vec3 p = isl->vertexPositionsPlanet[isl->xyToVertI(x, y)];

    //             if (length(p - camera->position) > horizonDistance + 15) continue;

    //             bool inView = false;
    //             camera->project(p, inView);
    //             if (inView) isl->isInView = true;
    //         }
    //     }
    //     if (isl->isInView) nrInView++;
    // }
}

bool PlanetCamera::cursorToLonLat(const glm::vec3 & rayDir, vec2 &lonLat, float offset) const
{
    glm::vec3 intersection, normal;
    if (glm::intersectRaySphere(camera->position, rayDir, plt->sphere.center, plt->sphere.radius - offset, intersection, normal)) {
        lonLat.x = glm::degrees(std::atan2(intersection.z, intersection.x) + 180.0f);
        lonLat.y = glm::degrees(glm::acos((intersection.y) / plt->sphere.radius));
        return true;
    } 
    else return false;
}