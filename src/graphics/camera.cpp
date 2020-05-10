#include "camera.hpp"

// Standard Headers
#include <utility>
#include <math.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>


// Local Headers
#include "common/planet.hpp"
#include "graphics/window_size.hpp"
#include "graphics/input/key_input.hpp"
#include "graphics/input/mouse_input.hpp"
#include "graphics/imgui/imgui.h"


Camera::Camera(float viewportWidth, float viewportHeight, float fov):
    direction(glm::vec3(0, 0, -1)),
    up(glm::vec3(0, 1, 0)),
    right(glm::vec3(1, 0, 0)),
    viewportWidth(viewportWidth), viewportHeight(viewportHeight),
    fov(fov)
{
}


void Camera::ShowDebugWindow(bool* p_open)
{
    ImGui::SetNextWindowSize(ImVec2(430,450), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Camera Debug", p_open))
    {
        ImGui::End();
        return;
    }

    struct funcs
    {
        static void ShowKeyValue(const char* key, float value) {
            ImGui::PushID(key);
            ImGui::Text("%s: %f", key, value);
            ImGui::PopID();
        }

        static void ShowKeyValue(const char* key, const char* value) {
            ImGui::PushID(key);
            ImGui::Text("%s: %s", key, value);
            ImGui::PopID();
            // if (node_open)
            // {
            //     static float dummy_members[8] = { 0.0f,0.0f,1.0f,3.1416f,100.0f,999.0f };
            //     for (int i = 0; i < 8; i++)
            //     {
            //         ImGui::PushID(i); // Use field index as identifier.
            //         if (i < 2)
            //         {
            //             ShowDummyObject("Child", 424242);
            //         }
            //         else
            //         {
            //             // Here we use a TreeNode to highlight on hover (we could use e.g. Selectable as well)
            //             ImGui::AlignTextToFramePadding();
            //             ImGui::TreeNodeEx("Field", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet, "Field_%d", i);
            //             ImGui::NextColumn();
            //             ImGui::SetNextItemWidth(-1);
            //             if (i >= 5)
            //                 ImGui::InputFloat("##value", &dummy_members[i], 1.0f);
            //             else
            //                 ImGui::DragFloat("##value", &dummy_members[i], 0.01f);
            //             ImGui::NextColumn();
            //         }
            //         ImGui::PopID();
            //     }
            //     ImGui::TreePop();
            // }
        }
    };

    funcs::ShowKeyValue("position", glm::to_string(position).c_str());
    funcs::ShowKeyValue("direction", glm::to_string(direction).c_str());
    funcs::ShowKeyValue("up", glm::to_string(up).c_str());
    funcs::ShowKeyValue("z_far", last_z_far);
    funcs::ShowKeyValue("z_near", last_z_near);

    ImGui::End();
}

void Camera::lookAt(vec3 target)
{
    lookAt(target, mu::Y);
}

void Camera::lookAt(vec3 target, vec3 localYAxis)
{
    direction = glm::normalize(target - position);
    right = glm::normalize(glm::cross(direction, localYAxis));
    up = glm::normalize(glm::cross(right, direction));
}

void Camera::orientUp(vec3 new_up)
{
    up = new_up;
    right = glm::normalize(glm::cross(direction, new_up));
}


void Camera::rotate(float degrees, glm::vec3 axis)
{
    float rads =  glm::radians(degrees);
    direction = glm::rotate(direction, rads, axis);
    right = glm::rotate(right, rads, axis);
    up = glm::rotate(up, rads, axis);
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

vec3 Camera::project(const vec3 &p, bool &inViewport) const
{
    vec4 homogeneous = combined * vec4(p, 1);
    vec3 h3 = homogeneous / homogeneous.w;
    if (!inViewport && homogeneous.z >= 0 && all(lessThanEqual(h3, vec3(1))) && all(greaterThanEqual(h3, vec3(-1))))
        inViewport = true;
    return h3;
}

bool blah = true;

vec3 Camera::project(const vec3 &p) const
{
    return project(p, blah);
}

void Camera::calculate(float z_near, float z_far)
{
    last_z_near = z_near;
    last_z_far = z_far;
    projection = glm::perspective(glm::radians(fov), viewportWidth / viewportHeight, z_near, z_far);
    view = glm::lookAt(
        position,
        position + direction,
        up
    );
    combined = projection * view;
}
