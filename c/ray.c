#include "ray.h"

#include "algebra.h"

Ray3 ray3_new(Float3* position, Float3* direction) {
	Ray3 ray;
	ray.origin = *position;
	ray.direction = *direction;
	// float3_normalize(&ray.direction);
	return ray;
}

void ray3_move_along(Ray3* ray, float distance) {
	Float3 movement = float3_mul(&ray->direction, distance);
	ray->origin = float3_add(&ray->origin, &movement);
}

Ray2 ray2_new(Float2* position, Float2* direction) {
	Ray2 ray;
	ray.origin = *position;
	ray.direction = *direction;
	return ray;
}
