#include "plane.h"

#include <math.h>

#include "algebra.h"

Plane plane_new(const float a, const float b, const float c, const float d) {
	Plane plane;
	plane.normal = float3_new(a, b, c);
	float len = float3_length(&plane.normal);
	float3_div_eq(&plane.normal, len);
	plane.d = d / len;
	return plane;
}

Plane plane_from_points(const Float3* p1, const Float3* p2, const Float3* p3) {
	const float a1 = p2->x - p1->x;
	const float b1 = p2->y - p1->y;
	const float c1 = p2->z - p1->z;
	const float a2 = p3->x - p1->x;
	const float b2 = p3->y - p1->y;
	const float c2 = p3->z - p1->z;
	Plane plane = plane_new(b1 * c2 - b2 * c1, a2 * c1 - a1 * c2,
							a1 * b2 - b1 * a2, 0);
	plane.d = -float3_dot(&plane.normal, p1);
	return plane;
}

float plane_intersect_distance(const void* plane, const Ray3* ray) {
	const Plane* pln = (Plane*)plane;
	const float a = float3_dot(&pln->normal, &ray->direction);
	if (fabsf(a) < 1e-6) return -1;
	const float b = float3_dot(&pln->normal, &ray->origin) + pln->d;
	return -b / a;
}

Float3 plane_normal_normalized(const void* plane,
							   const __attribute__((unused)) Float3* point) {
	const Plane* pln = (Plane*)plane;
	return pln->normal;
}
