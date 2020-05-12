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
    state({glm::vec3(100, 100, -500), glm::vec3(0, 0, 1), glm::vec3(0, 1, 0), glm::vec3(1, 0, 0)}),
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

    funcs::ShowKeyValue("position", glm::to_string(state.position).c_str());
    funcs::ShowKeyValue("direction", glm::to_string(state.direction).c_str());
    funcs::ShowKeyValue("up", glm::to_string(state.up).c_str());
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
    state.direction = glm::normalize(target - state.position);
    state.right = glm::normalize(glm::cross(state.direction, localYAxis));
    state.up = glm::normalize(glm::cross(state.right, state.direction));
}

void Camera::orientUp(vec3 new_up)
{
    state.up = new_up;
    state.right = glm::normalize(glm::cross(state.direction, new_up));
}


void Camera::rotate(float degrees, glm::vec3 axis)
{
    float rads =  glm::radians(degrees);
    state.direction = glm::rotate(state.direction, rads, axis);
    state.right = glm::rotate(state.right, rads, axis);
    state.up = glm::rotate(state.up, rads, axis);
}

glm::vec3 Camera::getRayDirection(float viewportX, float viewportY) const
{
    glm::vec2 point = glm::vec2(viewportX / WindowSize::width, 1 - viewportY / WindowSize::height) * glm::vec2(2) - glm::vec2(1);

    glm::vec4 eyeCoords = inverse(projection) * glm::vec4(point.x, point.y, -1, 1);
    eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.f, 0);
    glm::vec3 worldCoords = inverse(view) * eyeCoords;

    return glm::normalize(worldCoords);
}

// glm::vec3 Camera::getRayDirection(float viewportX, float viewportY) const
// {
//     glm::vec2 point = glm::vec2((2.f * viewportX / viewportWidth) - 1.f, 1.f - (2.f * viewportY) / viewportHeight);

//     glm::vec4 rayView = glm::inverse(projection) * glm::vec4(point.x, point.y, -1, 1);
//     rayView = glm::vec4(rayView.x, rayView.y, -1.f, 0.f);

//     return glm::normalize(glm::inverse(view) * rayView);
// }

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

glm::quat RotationBetweenVectors(vec3 start, vec3 dest){
	start = normalize(start);
	dest = normalize(dest);

	float cosTheta = dot(start, dest);
	vec3 rotationAxis;

	if (cosTheta < -1 + 0.001f){
		// special case when vectors in opposite directions:
		// there is no "ideal" rotation axis
		// So guess one; any will do as long as it's perpendicular to start
		rotationAxis = cross(vec3(0.0f, 0.0f, 1.0f), start);
		if (glm::length2(rotationAxis) < 0.01 ) // bad luck, they were parallel, try again!
			rotationAxis = cross(vec3(1.0f, 0.0f, 0.0f), start);

		rotationAxis = normalize(rotationAxis);
		return glm::angleAxis(glm::radians(180.0f), rotationAxis);
	}

	rotationAxis = cross(start, dest);

	float s = sqrt( (1+cosTheta)*2 );
	float invs = 1 / s;

	return quat(
		s * 0.5f, 
		rotationAxis.x * invs,
		rotationAxis.y * invs,
		rotationAxis.z * invs
	);

}

void Camera::moveTo(CameraState newState, bool calculateAnimation) {

    // Find the rotation between the front of the object (that we assume towards +Z,
    // but this depends on your model) and the desired direction
    quat rot1 = RotationBetweenVectors(mu::Z, newState.direction);
    
    // Because of the 1rst rotation, the up is probably completely screwed up.
    // Find the rotation between the "up" of the rotated object, and the desired up
    quat rot2 = RotationBetweenVectors(rot1 * mu::Y, newState.up);
    targetRotation = rot2 * rot1;

    targetPosition = newState.position;
    

    // targetRotation = glm::quat(glm::lookAt(
    //     newState.position,
    //     newState.position + newState.direction,
    //     newState.up
    // ));

    // state.position = newState.position;
    // state.direction = newState.direction;
    // state.right = newState.right;
    // state.up = newState.up;

    if (calculateAnimation) {
        
        float dist = glm::distance(newState.position, state.position);
        posTotalTime = dist / MAX_SPEED;
        posT = 0; 

        lastPosition = state.position;

        // Find the rotation between the front of the object (that we assume towards +Z,
        // but this depends on your model) and the desired direction
        quat rot1 = RotationBetweenVectors(mu::Z, state.direction);
        
        // Because of the 1rst rotation, the up is probably completely screwed up.
        // Find the rotation between the "up" of the rotated object, and the desired up
        quat rot2 = RotationBetweenVectors(rot1 * mu::Y, state.up);
        lastRotation = rot2 * rot1;
    }
}

bool blah = true;
glm::vec3 Camera::project(const vec3 &p) const
{
    return project(p, blah);
}

void Camera::calculate(float z_near, float z_far, float dt)
{
    last_z_near = z_near;
    last_z_far = z_far;
    projection = glm::perspective(glm::radians(fov), viewportWidth / viewportHeight, z_near, z_far);

    // If currently animating
    if (posT < posTotalTime) {
        posT += dt;

        float factor = clamp(posT / posTotalTime, 0.f, 1.f);
        state.position = glm::mix(lastPosition, targetPosition, factor);

        glm::quat interRot = glm::slerp(lastRotation, targetRotation, factor);
        state.direction = interRot * mu::Z;
        state.up = interRot * mu::Y;
        state.right = glm::cross(state.direction, state.up);

        if (posT > posTotalTime) {
            posT = posTotalTime = 0;
        }
    } else {

        state.position = targetPosition;
        state.direction = targetRotation * mu::Z;
        state.up = targetRotation * mu::Y;
        state.right = glm::cross(state.direction, state.up);
    }

    view = glm::lookAt(
        state.position,
        state.position + state.direction,
        state.up
    );
    
    combined = projection * view;
}
