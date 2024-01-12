#include "sphere.h"

#include <math.h>

Sphere sphere_new(const Float3* center, const float radius) {
	Sphere sphere;
	sphere.center = *center;
	sphere.radius = radius;
	return sphere;
}

float sphere_intersect_distance(const void* sphere, const Ray3* ray) {
	const Sphere* sph = (Sphere*)sphere;
	const Float3 point = float3_sub(&ray->origin, &sph->center);
	const float a = float3_dot(&ray->direction, &ray->direction);
	const float b = 2 * float3_dot(&point, &ray->direction);
	const float c = float3_dot(&point, &point) - sph->radius * sph->radius;
	const float discriminant = b * b - 4 * a * c;
	if (discriminant < 0.0) return -1;
	const float sqrt_discriminant = sqrt(discriminant);
	if (-b - sqrt_discriminant > 0.0) return (-b - sqrt_discriminant) / (2 * a);
	else return (-b + sqrt_discriminant) / (2 * a);
}

Float3 sphere_normal_vector(const void* sphere, const Float3* point) {
	const Sphere* sph = (Sphere*)sphere;
	return float3_sub(point, &sph->center);
}
