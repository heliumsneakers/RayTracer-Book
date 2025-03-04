#pragma once

#include "hittable.h"
#include "interval.h"
#include "hittable.h"
#include "stdio.h"
#include "vec3.h"
#include <cmath>

class sphere : public hittable {
	public:

	sphere(const point3& center, double radius) : center(center), radius(std::fmax(0, radius)) {}

	bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
		vec3 oc = center - r.origin();
		auto a = r.direction().length_squared();
		auto h = dot(r.direction(), oc);
		auto c = oc.length_squared() - radius * radius;
		/* Turns out we use the quadratic equation in ray sphere intersection, I'm intrigued!
		* Note: The original implementation has been simplified, but it still remains (-b +- sqrt(b^2 - 4ac) / 2a) at its core. */
		auto bhaskara = h*h - a*c;

		if (bhaskara < 0)
			return false;

		auto sqrtd = std::sqrt(bhaskara);

		// Find nearest root that lies in acceptable range
		auto root = (h - sqrtd) / a;
		if(!ray_t.surrounds(root)) {
			root = (h + sqrtd) / a;
			if(!ray_t.surrounds(root)) 
				return false;
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);

		return true;
	}

	private:
	point3 center;
	double radius;
};
