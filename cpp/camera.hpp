#pragma once

#include "ray.hpp"
#include "types.hpp"
#include <vector>

class Camera {
   public:
	std::vector<Ray> rays;

	Camera(Ray &Ray, f32 alpha, u64 n_rays_x, u64 n_rays_y,
		   u64 sqrt_ray_per_pixel);
};
