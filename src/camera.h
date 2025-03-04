#pragma once

#include <iostream>
#include "rt.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "hittable.h"
#include "interval.h"

class camera {
	public:
	double aspect_ratio	= 1.0;
	int image_width		= 100;
	int samples_ppx		= 10; // samples per pixel

	void initialize() {
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		auto focal_length	= 1.0;
		auto viewport_height	= 2.0;
		auto viewport_width	= viewport_height * (double(image_width) / image_height);
		camera_center = point3(0, 0, 0);


		// Calculate horizontal and vertical vectors along the image in viewport
		auto viewport_u		= vec3(viewport_width, 0, 0);
		auto viewport_v		= vec3(0, -viewport_height, 0); // -height becuase we are computing to the right on x and downwards on y from the top left origin.

		// Calculate horizontal and vertical delta vectors.
		pixel_delta_u	= viewport_u / image_width;
		pixel_delta_v	= viewport_v / image_height;

		// Calculate location of upper left pixel
		// Right handed axis for the renderer. Y up and Z towards the viewer.
		auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - viewport_u / 2 - viewport_v / 2;
		pixel00_loc		 = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

			std::clog << "\rINIT COMPLETE!\n";
	}

	void render(const hittable& world) {
		// Renderer
		std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
	
		for(int	j = 0; j < image_height; j++) {
		std::clog << "\rScan lines remaining: " << (image_height - j) << " \n" << std::flush;
			for(int i = 0; i < image_width; i++) {
				auto pixel_camera_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
				auto ray_direction = pixel_camera_center - camera_center;

				ray r(camera_center, ray_direction);

				color pixel_color = ray_color(r, world);

				write_color(std::cout, pixel_color);
			}
		}
		std::clog << "\rDone.		\n";
	}

	private:
	int image_height;
	point3 camera_center;
	point3 pixel00_loc;
	vec3 pixel_delta_u;
	vec3 pixel_delta_v;

	// returns ray color that hit the pixel.
	color ray_color (const ray& r, const hittable& world) const {
		hit_record rec;
		if (world.hit(r, interval(0, infinity), rec)) {
			return 0.5 * color(rec.normal.x() + 1, rec.normal.y() + 1, rec.normal.z() + 1); // for some reason the (rec.normal + color(1,1,1)) was not working so this was a proper solution.
		}

		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * color(1.0,1.0,1.0) + a * color(0.5, 0.7, 1.0); // Linear Interpolation to achieve gradient.
	}
};
