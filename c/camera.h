#pragma once

#include "algebra.h"
#include "ray.h"

typedef struct _Camera {
	Ray3 upper_left;
	Float3 delta_x, delta_y, d_x, d_y;
	int width, height, sqrt_ray_per_pixel;
} Camera;

Camera camera_new(Float3* position, Float3* direction, float angle, int pixel_x,
				  int pixel_y, int sqrt_ray_per_pixel);
