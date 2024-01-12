#include "plane.h"

#include <math.h>

Plane plane_new(float a, float b, float c, float d) {
	Plane plane;
	plane.normal = float3_new(a, b, c);
	plane.d = d;
	return plane;
}

Plane plane_from_points(Float3* p1, Float3* p2, Float3* p3) {
	float a1 = p2->x - p1->x;
	float b1 = p2->y - p1->y;
	float c1 = p2->z - p1->z;
	float a2 = p3->x - p1->x;
	float b2 = p3->y - p1->y;
	float c2 = p3->z - p1->z;
	Plane plane;
	plane.normal.x = b1 * c2 - b2 * c1;
	plane.normal.y = a2 * c1 - a1 * c2;
	plane.normal.z = a1 * b2 - b1 * a2;
	plane.d = -float3_dot(&plane.normal, p1);
	return plane;
}

float plane_intersect_distance(void* plane, Ray3* ray) {
	Plane* pln = (Plane*)plane;
	float a = float3_dot(&pln->normal, &ray->direction);
	if (fabsf(a) < 1e-6) return -1;
	float b = float3_dot(&pln->normal, &ray->origin) + pln->d;
	float t = -b / a;
	return t;
}

Float3 plane_normal_vector(void* plane, __attribute__((unused)) Float3* point) {
	Plane* pln = (Plane*)plane;
	return pln->normal;
}
