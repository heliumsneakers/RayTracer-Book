#pragma once

#include <memory>
#include <limits>
#include <cstdlib>

using std::make_shared;
using std::shared_ptr;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline double deg2rad(double degrees) {
	return degrees * pi / 180.0;
}

inline double random_double() {
	// Returns rand real in [0, 1)
	return std::rand() / (RAND_MAX + 1.0);
}

inline double rand_db(double min, double max) {
	// returns random real in [min, max)
	return min + (max - min) * random_double();
}
