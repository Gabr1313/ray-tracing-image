#pragma once

#include "algebra.h"
#include "ray.h"

typedef struct _Camera {
	Ray3 upper_left;
	Float3 delta_x, delta_y, d_x, d_y;
	int width, height, sqrt_ray_per_pixel;
} Camera;

Camera camera_new(const Float3* position, const Float3* direction,
				  const float angle, int const pixel_x, const int pixel_y,
				  const int sqrt_ray_per_pixel);
