#include "color.h"
#include "interval.h"
#include <iostream>
#include <ostream>

void write_color(std::ostream& out, const color& pixel_color) {
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	// Convert [0,1] values to the [0,255] byte range
	static const interval intensity(0.000, 0.999);
	int rbyte = int(256 * intensity.clamp(r));
	int gbyte = int(256 * intensity.clamp(g));
	int bbyte = int(256 * intensity.clamp(b));

	// output pixel color components
	out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
