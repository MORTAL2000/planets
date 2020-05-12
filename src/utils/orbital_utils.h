
#pragma once

#include <stdlib.h>
#include <string>
#include <math.h>

#include "unit_utils.h"
#include "common/orbital_mass.hpp"

#include "glm/glm.hpp"
#include "glm/gtx/rotate_vector.hpp"
// #include "glm/gtx/vector_angle.hpp"
// #include "glm/gtc/constants.hpp"
// #include "glm/gtx/string_cast.hpp"
// #include "glm/gtc/matrix_transform.hpp"

namespace ou
{

const glm::vec3
    X_AXIS = glm::vec3(1, 0, 0),
    Y_AXIS = glm::vec3(0, 1, 0),
    Z_AXIS = glm::vec3(0, 0, 1),
    ZERO_3 = glm::vec3(0);

const double GRAVITATIONAL_CONSTANT = 62500.f;
const float ORBIT_RESOLUTION = 2.f;

// calculateEffectiveParams
inline void calculateEffectiveParams(OrbitalMass * mass) {
	// Find the axial tilt
	glm::vec3 axialTilt = mass->orbitalAxis;

	// This is the nodes vector - points at AN
    glm::vec3 n = glm::cross(Z_AXIS, axialTilt);

	if (axialTilt.x == 0 && axialTilt.y == 0) { // If it points straight up, deflect marginally to avoid divide by zero errors
		axialTilt = glm::vec3(0, 0.0000000000001, 1);
	}

	// Calculate the new axes
	float iAxis = (glm::degrees(acos(axialTilt[2] / glm::length(axialTilt))));
	float axisAN = fmod(360 + (glm::degrees(acos(n[0] / glm::length(n)))), 360.f);

	if (n.y < 0) { // Flip to loAN if it is around the way, the inverse cos function can't explain everything
		axisAN = 360 - axisAN;
	}

	// if (isNaN(axisAN)) { // If inclination is zero - this can be anything, but zero is easiest
	// 	axisAN = 0;
	// }

	// Find the effective longitude of the ascending node and inclination
    mass->loANeff = axisAN;
    mass->ieff = iAxis;
}

inline glm::vec3 calculateAxialTilt(OrbitalMass * body) { // Rotate the angular momentum vector by the difference between vertical and the center's axis to find the new angular momentum vector

	// Get planetary data
	float i = glm::radians(body->i); 

	// Initialise the original angular momentum
	glm::vec3 originalAngMom = glm::vec3(0.f);

	// Calculate how far around the AN and inclination are for use in the later sections
    float ANdegree = glm::radians(fmod(360.f + body->loAN, 360.f));

	// Turn the orbital parameters into an angular momentum vector (reversing paramsFromVec calcs)
	float dist = cos(M_PI / 2.f - i);
	float height = sin(M_PI / 2.f - i);

	// Calculate needed sections. The angular momentum points 270 degrees (anticlockwise) from the AN
	float orgX = sin(ANdegree);
	float orgY = -cos(ANdegree);
	float orgZ = height;

	// Calculate angular momentum unit vector
	originalAngMom.x = orgX * dist;
	originalAngMom.y = orgY * dist;
	originalAngMom.z = orgZ;
	originalAngMom = glm::normalize(originalAngMom); // Note: Because I'm just looking at direction, the angular momentum is moderated to be 1 - a unit vector

	// Initialise major axes
    glm::vec3 axialTilt = body->axialTilt.value_or(Y_AXIS);

	// This finds where it is rotated and how much by
	glm::vec3 rotationAxis = glm::cross(Z_AXIS, axialTilt); // Find the axis to rotate about: Cross product of the axial tilt and "up"
	float rotationDegree = acos(glm::dot(Z_AXIS, axialTilt)); // Find how many degrees to rotate by

	// Transform it into a unit vector
	rotationAxis = glm::normalize(rotationAxis);

    // Rotate the vector
    return glm::normalize(glm::rotate(originalAngMom, glm::radians(rotationDegree), rotationAxis));
}

inline glm::vec3 calculateOrbitalPositionVector(OrbitalMass * body, float degree) { // Given which degree a planet is at, return the position vector

	// Get orbital data
	float e = body->e; // Eccentricity 
	float i = body->i; // Inclination
	float a = body->a; // No idea tbh
	float loPE = body->loPE;
	float loAN = body->loAN;
	// float center = mass->center;

	// Eccentric degree is how far away it is from the periapsis (trueAnomaly?)
	float trueAnomaly = fmod(360.f + degree + loPE, 360.f);

	// Find out the magnitude of the position vector
	float distance = a * (1 - e * e) / (1 + e * cos(glm::radians(trueAnomaly)));

	// Convert it into needed formats
	float degreesFromAN = glm::radians(-degree - loAN);
	float o = glm::radians(loAN);
	i = glm::radians(i);

	// Calculate the X, Y and Z components
	float x = distance * (cos(o) * cos(degreesFromAN) - sin(o) * sin(degreesFromAN) * cos(i));
	float y = distance * (sin(degreesFromAN) * sin(i));
	float z = distance * (sin(o) * cos(degreesFromAN) + cos(o) * sin(degreesFromAN) * cos(i));

	// Deal with axial tilts of moons (the moon/luna are excepted due to its odd orbit)
	// The only parameters I could find were discounting axial tilt - I think due to how odd it is
	// if (center != "sun" && name != "luna" && name != "the moon" && name != "ship") {

    if (body->axialTilt.has_value()) {
        // Calculates angular momentum vector, rotate by the rotation of the parent axial tilt from Z+
        // Then recalculate moderated orbital parameters based on this angular momentum

        float axisAN;
        float iAxis;

        if (!body->loANeff || !body->ieff) {
            calculateEffectiveParams(body);
        }

        axisAN = body->loANeff;
        iAxis = body->ieff;

        // Recalculate position - see earlier in the program
        float degreesFromAxisAN = glm::radians(-degree - axisAN);
        float oAxis = glm::radians(axisAN);
        iAxis = glm::radians(iAxis);
        x = distance * (cos(oAxis) * cos(degreesFromAxisAN) - sin(oAxis) * sin(degreesFromAxisAN) * cos(iAxis));
        y = distance * (sin(degreesFromAxisAN) * sin(iAxis));
        z = distance * (sin(oAxis) * cos(degreesFromAxisAN) + cos(oAxis) * sin(degreesFromAxisAN) * cos(iAxis));
    }
	// }

	// Return position
	return glm::vec3(x, y, z);
}

inline void generateOrbitalCoords(OrbitalMass * mass) {
    mass->orbitalPositions.clear();
    
    if (!mass->center) return;
    
    for (int i = 0; i < 360 * ou::ORBIT_RESOLUTION; i++) {
        float degree = i / ou::ORBIT_RESOLUTION;

        OrbitalState state;

        state.position = calculateOrbitalPositionVector(mass, degree);

        mass->orbitalPositions.push_back(state);
    }
}

// Returns the circulation time according to kepler's third law. 
// Takes in the length of the major axis, the mass of the central body, and mass of our body 
inline float findPeriod(float majorAxis, double centerGravParam, double gravParam) {
	// Calculate from Keplar's second law
    return sqrt((pow(majorAxis, 3) * 4 * pow(M_PI, 2)) / (centerGravParam + gravParam));
}

inline void generateOrbitalTimes(OrbitalMass * mass) {
    // Get the correct data
	float gravitationalParameter = mass->center->gravParam;

    // Initialise iterator variables
	float timesum = 0;
    for (int i = 0; i < 360 * ou::ORBIT_RESOLUTION; i++) {
        // Move the degree forward

        OrbitalState & state = mass->orbitalPositions[i];

		// Find the positions, and the distance between
		glm::vec3 currentPosition = state.position;
		glm::vec3 nextPosition = mass->orbitalPositions[(i + 1) % int(360 * ou::ORBIT_RESOLUTION)].position;
        state.distance = glm::distance(currentPosition, nextPosition);

		// Find the velocity at this point
		state.speed = sqrt(gravitationalParameter * ((2.f / glm::length(currentPosition)) - (1.f / mass->a)));
        state.dt = state.distance / state.speed;
        
		// Additive time calculated by how long it takes to get between that small segment
		timesum += state.dt;
        state.time = timesum;
    }

    // Finish it off by making the last be the full period
//	 degree = fmod(round((L) * ORBIT_RESOLUTION) - 1 + 360 * ORBIT_RESOLUTION, 360 * ORBIT_RESOLUTION);
//	 mass->orbitalTimes[degree] = findPeriod(a, mass->center->gravParam, mass->gravParam);
}

inline glm::vec3 findPlanetLocation(OrbitalMass * body, float time) {

    float period = body->orbitalPositions.back().time; //findPeriod(body->a, body->center->gravParam, body->gravParam);
    float sinceEpoch = fmod(time, period);

    OrbitalState current = body->orbitalPositions[0], next = body->orbitalPositions[0];

    // Linear search through orbital positions
    for (size_t i = 0; i < body->orbitalPositions.size(); i++) {
        OrbitalState state = body->orbitalPositions[i];
        
        if (state.time <= sinceEpoch) {
            current = state;
        }
        
        if (state.time > sinceEpoch) {
            next = state;
            break;
        }
    }

    // Interop position
    float remaining = sinceEpoch - current.time;
    return glm::mix(current.position, next.position, remaining / current.dt);
}


} // namespace mu
