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
	int max_depth		= 10; // max recursion depth / max ray bounces in the scene
	

	void initialize() {
		image_height = int(image_width / aspect_ratio);
		image_height = (image_height < 1) ? 1 : image_height;

		auto focal_length	= 1.0;
		auto viewport_height	= 2.0;
		auto viewport_width	= viewport_height * (double(image_width) / image_height);

		pixel_sample_scale = 1.0 / samples_ppx;

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
				color pixel_color(0,0,0);
				for (int sample = 0; sample < samples_ppx; sample++) {
					ray r = get_ray(i, j);
					pixel_color += ray_color(r, max_depth, world);
				}
				write_color(std::cout, pixel_sample_scale * pixel_color);
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
	double pixel_sample_scale;

	ray get_ray(int i, int j) const {
		// construct camera ray from origin and in the direction of a randomly sampled point around the pixel location i,j
		auto offset = sample_square();
		auto pixel_sample = pixel00_loc
					+ ((i + offset.x()) * pixel_delta_u)
					+ ((j + offset.y()) * pixel_delta_v);
		auto ray_origin = camera_center;
		auto ray_direction = pixel_sample - ray_origin;
		return ray(ray_origin, ray_direction);
	}

	vec3 sample_square() const {
		// returns the vector to a random point in the [-.5,-.5] - [+.5,+.5] unit square
		return vec3(random_double() - 0.5, random_double() - 0.5, 0);
	}

	// returns ray color that hit the pixel.
	color ray_color (const ray& r, int depth, const hittable& world) const {
		if (depth <= 0) {
			// Stop gathering color info if we reach max bounces this can cause memory corruption
			return color(0,0,0);
		}

		hit_record rec;

		if (world.hit(r, interval(0.001, infinity), rec)) {
			vec3 dir = rec.normal + rand_on_hemi(rec.normal);
			// Here the float multiplication is the reflectance. Also known as light absorption. 
			return 0.5 * ray_color(ray(rec.p, dir), depth - 1, world);
		}

		vec3 unit_direction = unit_vector(r.direction());
		auto a = 0.5 * (unit_direction.y() + 1.0);
		return (1.0 - a) * color(1.0,1.0,1.0) + a * color(0.5, 0.7, 1.0); // Linear Interpolation to achieve gradient.
	}
};
