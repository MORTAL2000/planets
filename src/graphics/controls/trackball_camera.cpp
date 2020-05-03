#include "planet_camera.hpp"

// Standard Headers
#include <math.h>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/intersect.hpp>
#include <glm/gtx/quaternion.hpp>

// Local Headers
#include "graphics/window_size.hpp"
#include "graphics/input/key_input.hpp"
#include "graphics/input/mouse_input.hpp"

#define   SQRT1_2  0.7071067811865476

PlanetCamera::PlanetCamera(Camera *cam, Planet *plt)
    : camera(cam), plt(plt),
		m_rotateSpeed(1/plt->config.radius),
		m_zoomSpeed(1.2f),
		m_noRoll(false),
		m_staticMoving(false),
		m_dynamicDampingFactor(0.2f),
		m_minDistance(0.0f),
		m_maxDistance(std::numeric_limits<float>::infinity()),
		m_target(0.0f),
		m_lastPos(0.0f),
		m_eye(0.0f),
		m_moveCurr(0.0f),
		m_movePrev(0.0f),
        m_lastAngle(0)
{
}

const int DRAG_BUTTON = GLFW_MOUSE_BUTTON_LEFT;

void PlanetCamera::update(float dt)
{
    m_eye = camera->position - m_target;

    RotateCamera();

    camera->position = (m_target + m_eye);

    // CheckDistances();

    camera->lookAt(m_target);

    if (glm::length2(m_lastPos - camera->position) > 0.0f) 
    {
        m_lastPos = camera->position;
    }
    // auto prevPos = camera->position;

    // ImGuiIO& io = ImGui::GetIO();

    // if (KeyInput::pressed(GLFW_KEY_W))
    //     camera->position += camera->direction * glm::vec3(dt * speedMultiplier);

    // if (KeyInput::pressed(GLFW_KEY_S))
    //     camera->position += camera->direction * glm::vec3(-dt * speedMultiplier);

    // if (KeyInput::pressed(GLFW_KEY_D))
    //     camera->position += camera->right * glm::vec3(dt * speedMultiplier);

    // if (KeyInput::pressed(GLFW_KEY_A))
    //     camera->position += camera->right * glm::vec3(-dt * speedMultiplier);

    // if (KeyInput::pressed(GLFW_KEY_Q))
    //     camera->rotate(-dt * speedMultiplier, camera->direction);

    // if (KeyInput::pressed(GLFW_KEY_E))
    //     camera->rotate(dt * speedMultiplier, camera->direction);
    
    bool startDrag = MouseInput::justPressed(DRAG_BUTTON),
         dragging = MouseInput::pressed(DRAG_BUTTON),
         stoppedDragging = MouseInput::justReleased(DRAG_BUTTON);

    // if (MouseInput::mouseX < 0 || MouseInput::mouseY < 0 || MouseInput::mouseX > WindowSize::width || MouseInput::mouseY > WindowSize::height) {
    //     startDrag  = false;
    //     dragging = false;
    //     stoppedDragging = false;
    // }
    
    if (dragging || startDrag) {
    // {
        if (startDrag) {
            m_moveCurr = glm::vec3(getMouseOnCircle(MouseInput::mouseX, MouseInput::mouseY), 0);
			m_movePrev = m_moveCurr;
        } else {
            m_movePrev = m_moveCurr;
            m_moveCurr = glm::vec3(getMouseOnCircle(MouseInput::mouseX, MouseInput::mouseY), 0);
        }
    }
    else
    {
    //     draggingStarted = false;
    //     // bool
    //     //     up = MouseInput::mouseY < 35,
    //     //     down = MouseInput::mouseY > WindowSize::height - 35,
    //     //     left = MouseInput::mouseX < 35,
    //     //     right = MouseInput::mouseX > WindowSize::width - 35;

    //     // float moveSpeed = 50. - actualZoom * 20.;

    //     // if (up) lat -= dt * moveSpeed;
    //     // else if (down) lat += dt * moveSpeed;

    //     // if (left) lon -= dt * moveSpeed;
    //     // else if (right) lon += dt * moveSpeed;

    //     // Slowly drift to an orbit after a timeout

    //     if (stoppedDragging) afterDragTimer = 1;

    //     if (afterDragTimer > 0)
    //     {
    //         glm::vec2 d = dragVelocity() * glm::vec2(afterDragTimer * dt * 20.);
    //         lon += d.x;
    //         lat += d.y;
    //         afterDragTimer -= dt;
    //     }
    }


    // lon = fmod(lon, 360.f);
    // lat = fmax(0.f, fmin(180.f, lat));

    // zoom = fmin(.92 , fmax(0., zoom + MouseInput::yScroll * .08));
    // float prevActualZoom = actualZoom;
    // actualZoom = actualZoom * (1. - dt * 10.) + zoom * dt * 10.;

    // zoomVelocity = abs(prevActualZoom - actualZoom) / dt;

    // camera->position = mu::Y * glm::vec3(5. + 235 * (1. - actualZoom));
    // camera->position.z += actualZoom * 24.5;

    // camera->lookAt(mu::ZERO_3, -mu::Z);

    // glm::vec3 translateCam = glm::vec3(0, plt->config.radius, 0);

    // glm::quat lonTransform = glm::angleAxis(glm::radians(lon), mu::Y);
    // glm::quat latTransform = glm::angleAxis(glm::radians(lat), mu::X);
    // glm::mat4 transform = glm::toMat4(lonTransform * latTransform); // * glm::translate(glm::mat4(1.0), translateCam);

    // camera->position = transform * glm::vec4(camera->position, 1);
    // camera->direction = transform * glm::vec4(camera->direction, 0);
    // camera->right = transform * glm::vec4(camera->right, 0);
    // camera->up = transform * glm::vec4(camera->up, 0);

    camera->update();
    // if (prevPos != camera->position) planetCulling();
}

glm::vec2 PlanetCamera::getMouseOnCircle(int clientX, int clientY)
{
    return glm::vec2(
        ((float)clientX - (float)WindowSize::width * 0.5f) / (float)(WindowSize::width * 0.5f),
        ((float)WindowSize::height * 0.5f - (float)clientY) / (float)(WindowSize::height * 0.5f)
    );
}

void PlanetCamera::RotateCamera() {
    glm::vec3 moveDirection(m_moveCurr.x - m_movePrev.x, m_moveCurr.y - m_movePrev.y, 0);
    float angle = moveDirection.length();
    if ( m_moveCurr !=  m_movePrev) {

        m_eye = camera->position - m_target;

        glm::vec3 cameraDirection = glm::normalize(m_eye);
        glm::vec3 cameraUp = glm::normalize(camera->up);
        glm::vec3 cameraSideways = glm::normalize(glm::cross(cameraUp, cameraDirection));

        cameraUp *= m_moveCurr.y - m_movePrev.y;
        cameraSideways *= m_moveCurr.x - m_movePrev.x;

        moveDirection = cameraUp + cameraSideways;
        glm::vec3 axis = glm::normalize(glm::cross(moveDirection, m_eye));

        angle *= m_rotateSpeed;
        glm::quat quaternion = glm::angleAxis(angle, axis);

        m_eye = glm::rotate(quaternion, m_eye);
        camera->up = glm::rotate(quaternion, camera->up);

        m_lastAxis = axis;
        m_lastAngle = angle;

    } else if ( ! m_staticMoving && m_lastAngle) {

        m_lastAngle *= sqrt( 1.0 - m_dynamicDampingFactor);

        m_eye = camera->position - m_target;
        glm::quat quaternion = glm::angleAxis(m_lastAngle, m_lastAxis);
        
        m_eye = glm::rotate(quaternion, m_eye);
        camera->up = glm::rotate(quaternion, camera->up);
    }

    m_movePrev = m_moveCurr;
}

void PlanetCamera::updateHorizonDistance()
{
    // horizonDistance = sqrt(pow(length(camera->position), 2) - pow(plt->config.radius, 2));
}

// void PlanetCamera::ZoomCamera()
// {
// 	float factor = 1.0f + (float)(m_zoomEnd.y - m_zoomStart.y) * m_zoomSpeed;

//     if (factor != 1.0f && factor > 0.0f) {
//         m_eye = m_eye * (float)factor;

//         if (m_staticMoving) {
//             m_zoomStart = m_zoomEnd;
//         } else {
//             m_zoomStart.y += (float)(m_zoomEnd.y - m_zoomStart.y) * m_dynamicDampingFactor;
//         }
//     }
// }

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

// bool PlanetCamera::cursorToLonLat(const glm::vec3 & rayDir, vec2 &lonLat, float offset) const
// {
//     glm::vec3 intersection, normal;
//     if (glm::intersectRaySphere(camera->position, rayDir, glm::vec3(0.f), plt->config.radius - offset, intersection, normal)) {
//         lonLat.x = glm::degrees(std::atan2(intersection.z, intersection.x) + 180.0f);
//         lonLat.y = glm::degrees(glm::acos((intersection.y) / plt->config.radius));
//         return true;
//     } 
//     else return false;
// }
