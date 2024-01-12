#include "camera.hpp"

#include <array>
#include <cassert>
#include <cmath>

#include "algebra.hpp"
#include "ray.hpp"

Camera::Camera(Ray &ray, f32 alpha, u64 n_rays_x, u64 n_rays_y,
			   u64 sqrt_ray_per_pixel) {
	assert(0.0 <= alpha && alpha <= M_PI);

	f32 ratio = f32(n_rays_x) / n_rays_y;
	
	// todo: finire funzione
}

std::array<Vec3, 4> screen_corners(Ray &ray, f32 alpha, f32 ratio) {
	assert(ratio > 0);

	f32 alpha_2 = alpha / 2;
	Vec3 base = Vec3(1, 0, 0);

	f32 x, y, z;
	if (ratio < 1) {
		Vec3 yawed = Mat3::yaw(alpha_2) * base;
		x = yawed.x;
		z = yawed.z;
		y = z * ratio;
	} else {
		Vec3 rolled = Mat3::roll(alpha_2) * base;
		x = rolled.x;
		y = rolled.y;
		z = y / ratio;
	}

	Vec3 base_ul = Vec3(x, -y, z), base_ur = Vec3(x, y, z), base_dl = Vec3(x, -y, -z),
		 base_dr = Vec3(x, y, -z);
	
	Vec3 dir1 = ray.dir.normalized();
	Vec3 dir2 = Vec3(0, 0, 1).crossed(dir1).normalized();
	Vec3 dir3 = dir1.crossed(dir2).normalized();
	
	Mat3 change_of_basis({{{dir1.x, dir2.x, dir3.x},
						   {dir1.y, dir2.y, dir3.y},
						   {dir1.z, dir2.z, dir3.z}}});
	
	Vec3 pos = ray.orig;

	Ray ray_ul = Ray(pos, (change_of_basis * base_ul).normalized());
	Ray ray_ur = Ray(pos, (change_of_basis * base_ur).normalized());
	Ray ray_dl = Ray(pos, (change_of_basis * base_dl).normalized());
	Ray ray_dr = Ray(pos, (change_of_basis * base_dr).normalized());
	
	// TODO: plane...
}
