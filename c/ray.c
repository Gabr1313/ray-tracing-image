#include "ray.h"

#include "algebra.h"

Ray3 ray3_new(const Float3* position,const  Float3* direction) {
	Ray3 ray;
	ray.origin = *position;
	ray.direction = *direction;
	return ray;
}

void ray3_move_along(Ray3* ray,const  float distance) {
	Float3 movement = float3_mul(&ray->direction, distance);
	ray->origin = float3_add(&ray->origin, &movement);
}

Ray2 ray2_new(const Float2* position,const  Float2* direction) {
	Ray2 ray;
	ray.origin = *position;
	ray.direction = *direction;
	return ray;
}
