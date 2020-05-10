
#pragma once

namespace uu
{
    enum Distance { M, KM, AU, LY };
    enum Time { MS, S, Min, H, D, W, Mo, Y};
    enum Speed { C };

inline double convertTime(Time from, Time to, float power = 1) { // Convert any time from a format to a format with a given power
	double value = 1;

	if (from == Time::MS) {
		value = 1;
	}
	else if (from == Time::S) {
		value = 1000;
	}
	else if (from == Time::Min) {
		value = 1000 * 60;
	}
	else if (from == Time::H) {
		value = 1000 * 60 * 60;
	}
	else if (from == Time::D) {
		value = 1000 * 60 * 60 * 24;
	}
	else if (from == Time::W) {
		value = 1000 * 60 * 60 * 24 * 7;
	}
	else if (from == Time::Mo) {
		value = 1000 * 60 * 60 * 24 * 365.2422 / 12;
	}
	else if (from == Time::Y) {
		value = 1000 * 60 * 60 * 24 * 365.2422;
	}

//	if (to == Time::MS) {
//		value = value;
//	}
	else if (to == Time::S) {
		value = value / (1000);
	}
	else if (to == Time::Min) {
		value = value / (1000 * 60);
	}
	else if (to == Time::H) {
		value = value / (1000 * 60 * 60);
	}
	else if (to == Time::D) {
		value = value / (1000 * 60 * 60 * 24);
	}
	else if (to == Time::W) {
		value = value / (1000 * 60 * 60 * 24 * 7);
	}
	else if (to == Time::Mo) {
		value = value / (1000 * 60 * 60 * 24 * 365.2422 / 12);
	}
	else if (to == Time::Y) {
		value = value / (1000 * 60 * 60 * 24 * 365.2422);
	}

	return pow(value, power);
}

inline double convertDistance(Distance from, Distance to, float power = 1) { // Convert any distance from anything to anything, and with a power modifier

	double value = 1;

	if (from == Distance::M) {
		value = 1;
	}
	else if (from == Distance::KM) {
		value = 1000;
	}
	else if (from == Distance::AU) {
		value = 149597870700;
	}
	else if (from == Distance::LY) {
		value = 9460528436447506;
	}

//	if (to == Distance::M) {
//		value = value;
//	}
	else if (to == Distance::KM) {
		value = value / 1000;
	}
	else if (to == Distance::AU) {
		value = value / 149597870700;
	}
	else if (to == Distance::LY) {
		value = value / 9460528436447506;
	}

	return pow(value, power);
}

inline float AU3Y2toM3S2(float value) {
    return value * convertDistance(Distance::AU, Distance::M, 3) * convertTime(Time::Y, Time::S, -2);
}

inline float M3S2toAU3Y2(float value) {
    return value * convertDistance(Distance::M, Distance::AU, 3) * convertTime(Time::S, Time::Y, -2);
}


// inline float convertSpeed(Speeds from, Speeds to) { // Convert a speed in L/T format from anything to anything
// 	if (from == Speeds::C) {
// 		from = ["LY", "Y"]
// 	}
// 	else {
// 		from = from.split("/");
// 	}
// 	if (to == Speeds::C) {
// 		to = ["LY", "Y"]
// 	}
// 	else {
// 		to = to.split("/");
// 	}

// 	var Dfrom = from[0];
// 	var Tfrom = from[1];
// 	var Dto = to[0];
// 	var Tto = to[1];

// 	return convertDistance(Dfrom, Dto, 1) * convertTime(Tfrom, Tto, -1);

// }
}
