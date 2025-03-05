#include "rt.h"
#include "camera.h"
#include "hittable_list.h"
#include "sphere.h"
#include "vec3.h"

int main (void) {
	// World
	hittable_list world;

	world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
	world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));

	camera cam;
	cam.aspect_ratio	= 16.0 / 9.0;
	cam.image_width		= 1080;
	cam.samples_ppx		= 100;
	cam.max_depth		= 50;

	cam.initialize();
	cam.render(world);
}
