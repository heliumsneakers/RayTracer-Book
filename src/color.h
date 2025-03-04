#pragma once
#include "vec3.h"
#include <iostream>
#include <ostream>

using color = vec3;

void write_color(std::ostream& out, const color& pixel_color);
