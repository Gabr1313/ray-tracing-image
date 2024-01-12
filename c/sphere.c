#include "sphere.h"

#include <math.h>

Sphere sphere_new(Float3* center, float radius) {
	Sphere sphere;
	sphere.center = *center;
	sphere.radius = radius;
	return sphere;
}

float sphere_intersect_distance(void* sphere, Ray3* ray) {
	Sphere* sph = (Sphere*)sphere;
	Float3 point = float3_sub(&ray->origin, &sph->center);
	float a = float3_dot(&ray->direction, &ray->direction);
	float b = 2 * float3_dot(&point, &ray->direction);
	float c = float3_dot(&point, &point) - sph->radius * sph->radius;
	float discriminant = b * b - 4 * a * c;
	if (discriminant < 0.0) return -1;
	float sqrt_discriminant = sqrt(discriminant);
	if (-b - sqrt_discriminant > 0.0) return (-b - sqrt_discriminant) / (2 * a);
	else return (-b + sqrt_discriminant) / (2 * a);
}

Float3 sphere_normal_vector(void* sphere, Float3* point) {
	Sphere* sph = (Sphere*)sphere;
	return float3_sub(point, &sph->center);
}
