#pragma once

// Standard Headers
#include <array>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

const static std::array<glm::vec3, 3> global_axis = {
        glm::vec3(1.f, 0.f, 0.f),
        glm::vec3(0.f, 1.f, 0.f),
        glm::vec3(0.f, 0.f, 1.f)
};

struct orientation_state {
	glm::vec3 position;
	glm::quat rotation;
};


struct bounding_box {
	glm::vec3 max;
	glm::vec3 min;
};

// set the relevant elements of our oriented bounding box
class OBB
{
    glm::vec3 get_center(const bounding_box& bbox);
    glm::vec3 get_half_size(const bounding_box& bbox);
    bool getSeparatingPlane(const glm::vec3& RPos, const glm::vec3& Plane, const OBB & box2);
public:
    OBB(const bounding_box & bbox, const orientation_state & state);

    glm::vec3 center;
	glm::vec3 axis_x;
	glm::vec3 axis_y;
	glm::vec3 axis_z;
	glm::vec3 half_size;

    glm::quat rotation;

    bool test_obb_obb_collision(const OBB & box2);
};