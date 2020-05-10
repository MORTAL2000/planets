
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

const vec3
    X_AXIS = vec3(1, 0, 0),
    Y_AXIS = vec3(0, 1, 0),
    Z_AXIS = vec3(0, 0, 1),
    ZERO_3 = vec3(0);

const double MASS_SUN = 1.98855 * pow(10, 30);
const double GRAVITATIONAL_CONSTANT = 6.67408 * pow(10, -11);
const float ORBIT_RESOLUTION = 2.f;

// calculateEffectiveParams
inline void calculateEffectiveParams(OrbitalMass * mass) {
	// Find the axial tilt
	vec3 axialTilt = mass->orbitalAxis;

	// This is the nodes vector - points at AN
    vec3 n = glm::cross(Z_AXIS, axialTilt);

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

inline glm::vec3 calculateAxialTilt(OrbitalMass * mass) { // Rotate the angular momentum vector by the difference between vertical and the center's axis to find the new angular momentum vector

	// Get planetary data
	float i = glm::radians(mass->i); 

	// Initialise the original angular momentum
	glm::vec3 originalAngMom = glm::vec3(0.f);

	// Calculate how far around the AN and inclination are for use in the later sections
    float ANdegree = glm::radians(fmod(360.f + mass->loAN, 360.f));

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
	glm::vec3 axialTilt = mass->axialTilt;

	// This finds where it is rotated and how much by
	glm::vec3 rotationAxis = glm::cross(Z_AXIS, axialTilt); // Find the axis to rotate about: Cross product of the axial tilt and "up"
	float rotationDegree = acos(glm::dot(Z_AXIS, axialTilt)); // Find how many degrees to rotate by

	// Transform it into a unit vector
	rotationAxis = glm::normalize(rotationAxis);

    // Rotate the vector
    return glm::normalize(glm::rotate(originalAngMom, glm::radians(rotationDegree), rotationAxis));
}

inline glm::vec3 calculateOrbitalPositionVector(OrbitalMass * mass, float degree) { // Given which degree a planet is at, return the position vector

	// Get orbital data
	float e = mass->e;
	float i = mass->i;
//	float a = mass->a;
	float loPE = mass->loPE;
	float loAN = mass->loAN;
	// float center = mass->center;

	// Eccentric degree is how far away it is from the periapsis
	float eccentricDegree = fmod(360.f + degree + loPE, 360.f);

	// Find out the magnitude of the position vector
	float distance = mass->a * (1 - e * e) / (1 + e * cos(glm::radians(eccentricDegree)));

	// Convert it into needed formats
	float degreesFromAN = glm::radians(-degree - loAN);
	float o = glm::radians(loAN);
	i = glm::radians(i);

	// Calculate the X, Y and Z components
	float x = distance * (cos(o) * cos(degreesFromAN) - sin(o) * sin(degreesFromAN) * cos(i));
	float y = distance * (sin(o) * cos(degreesFromAN) + cos(o) * sin(degreesFromAN) * cos(i));
	float z = distance * (sin(degreesFromAN) * sin(i));

	// Deal with axial tilts of moons (the moon/luna are excepted due to its odd orbit)
	// The only parameters I could find were discounting axial tilt - I think due to how odd it is
	// if (center != "sun" && name != "luna" && name != "the moon" && name != "ship") {

    if (mass->hasAxialTilt) {
        // Calculates angular momentum vector, rotate by the rotation of the parent axial tilt from Z+
        // Then recalculate moderated orbital parameters based on this angular momentum

        float axisAN;
        float iAxis;

        if (!mass->loANeff || !mass->ieff) {
            calculateEffectiveParams(mass);
        }

        axisAN = mass->loANeff;
        iAxis = mass->ieff;

        // Recalculate position - see earlier in the program
        float degreesFromAxisAN = glm::radians(-degree - axisAN);
        float oAxis = glm::radians(axisAN);
        iAxis = glm::radians(iAxis);
        x = distance * (cos(oAxis) * cos(degreesFromAxisAN) - sin(oAxis) * sin(degreesFromAxisAN) * cos(iAxis));
        y = distance * (sin(oAxis) * cos(degreesFromAxisAN) + cos(oAxis) * sin(degreesFromAxisAN) * cos(iAxis));
        z = distance * (sin(degreesFromAxisAN) * sin(iAxis));
    }
	// }

	// Return position
	return glm::vec3(x, y, z);
}

inline void generateOrbitalCoords(OrbitalMass * mass) {
    mass->orbitalPositions.clear();
    for (float degree = 360; degree > 0; degree -= (1 / ORBIT_RESOLUTION)) {
        mass->orbitalPositions[degree] = ou::calculateOrbitalPositionVector(mass, degree);
    }
}

inline float findGravParam(OrbitalMass * center) {
    // Set default mass to sun
	double mass = MASS_SUN;

    mass = uu::AU3Y2toM3S2(4.f * M_PI * M_PI) / GRAVITATIONAL_CONSTANT;
    double gravitationalParameter = mass * GRAVITATIONAL_CONSTANT;

    if (center) {
        if (center->mass) {
            gravitationalParameter = center->mass * GRAVITATIONAL_CONSTANT;
        } else {
            gravitationalParameter = center->gravParam;
        }
    } 

    return gravitationalParameter;
}

inline float findPeriod(float a, OrbitalMass * center) { // Find the period of a planet given the center and semi-major axis

	// Find and convert the gravitational parameter of the center
	double gravitationalParameter = findGravParam(center);
	gravitationalParameter = uu::M3S2toAU3Y2(gravitationalParameter);

	// Calculate from Keplar's second law
	return pow(((4.f * pow(M_PI, 2)) / gravitationalParameter) * pow(a, 3), 0.5f); // Period in years. 1AU = 1 year
}

inline void generateOrbitalTimes(OrbitalMass * mass) {
    // Get the correct data
	float a = mass->a;
    float L = mass->rL;

	float gravitationalParameter = findGravParam(mass->center);
	gravitationalParameter = uu::M3S2toAU3Y2(gravitationalParameter);

    // The inital degree starts at its position at epoch - because the time is zero at 0 remainer time
    float degree = fmod((round((L) * ORBIT_RESOLUTION)), (360 * ORBIT_RESOLUTION));

    // Initialise storage variables
    mass->orbitalVelocities.clear();
    mass->orbitalTimes.clear();

    // Initialise iterator variables
	float timesum = 0;
    for (int counter = 1; counter < 360 * ORBIT_RESOLUTION; counter++) {
        // Move the degree forward
		degree = fmod((degree + 1), (360 * ORBIT_RESOLUTION));
		float currentDegree = fmod((degree), (360 * ORBIT_RESOLUTION));

        OrbitalVelocity result;

		// Find the positions, and the distance between
		glm::vec3 nextPosition = mass->orbitalPositions[degree];
		glm::vec3 currentPosition = mass->orbitalPositions[currentDegree];
        result.distance = glm::distance(currentPosition, nextPosition);

		// Find the velocity at this point
		result.speed = pow(gravitationalParameter * ((2 / glm::length(currentPosition)) - (1 / a)), 0.5);
        result.time = result.distance / result.speed;

		// Also store this velocity in the orbital velocities part
        mass->orbitalVelocities[degree] = result;
    
		// Additive time calculated by how long it takes to get between that small segment
		timesum += result.time;
        mass->orbitalTimes[degree] = timesum;
    }

    // Finish it off by making the last be the full period
	degree = fmod(round((L) * ORBIT_RESOLUTION) - 1 + 360 * ORBIT_RESOLUTION, 360 * ORBIT_RESOLUTION);
	mass->orbitalTimes[degree] = findPeriod(a, mass->center);
}

inline void correctParams(OrbitalMass * mass) {
    if (!mass->gravParam) {
        mass->gravParam = mass->mass * GRAVITATIONAL_CONSTANT;
    }

    // Calculate SOI - just trust the calculation, or look it up
	float centParam = findGravParam(mass->center);
    mass->SOI = mass->a * pow(mass->gravParam / (3 * centParam), 1.f / 3.f);

    // Set axial tilt to straight up if not specified
    if (!mass->hasAxialTilt) {
        mass->axialTilt = glm::vec3(0, 0, 1);
    }

    mass->orbitalAxis = calculateAxialTilt(mass);

    // // If no rotational epoch specified, set to zero
    // if (!planets[planet]["rotEpoch"]) {
    //     planets[planet]["rotEpoch"] = 0;
    // }

    // // Correct argument of periapsis to longitude of the periapsis (if applicable)
    // if (planets[planet]["aoPE"]) {
    //     planets[planet]["loPE"] = planets[planet]["aoPE"] + planets[planet]["loAN"]
    // }

    // // Correct mean longitude to real longitude at epoch (by modifying the epoch)
    // if (!planets[planet]["rL"]) {
    //     if (planets[planet]["L"]) {
    //         planets[planet]["M"] = planets[planet]["L"] - planets[planet]["loPE"]
    //     }
    //     var shift = findPeriod(planets[planet]["a"], planets[planet]["center"]) * convertTime("Y", "MS") * (planets[planet]["M"] / 360);
    //     var newEpoch = EPOCH.getTime();
    //     if (planets[planet]["epoch"]) {
    //         newEpoch = planets[planet]["epoch"].getTime();
    //     }
    //     newEpoch = new Date(newEpoch - shift);
    //     planets[planet]["epoch"] = newEpoch
    //     planets[planet]["rL"] = planets[planet]["loPE"]
    // }
}


} // namespace mu
