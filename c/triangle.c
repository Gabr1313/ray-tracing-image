#include "triangle.h"

#include <math.h>

#include "algebra.h"
#include "plane.h"

#define XY 1
#define YZ 2
#define ZX 3
int projection_type(const Plane* plane);

Triangle triangle_new(const Float3* p1, const Float3* p2, const Float3* p3) {
	const Plane plane = plane_from_points(p1, p2, p3);
	const int projection = projection_type(&plane);
	Float2 v1, v2, v3;
	if (projection == XY) {
		v1 = float2_new(p1->x, p1->y);
		v2 = float2_new(p2->x, p2->y);
		v3 = float2_new(p3->x, p3->y);
	} else if (projection == YZ) {
		v1 = float2_new(p1->y, p1->z);
		v2 = float2_new(p2->y, p2->z);
		v3 = float2_new(p3->y, p3->z);
	} else {
		v1 = float2_new(p1->z, p1->x);
		v2 = float2_new(p2->z, p2->x);
		v3 = float2_new(p3->z, p3->x);
	}

	const Float2 s1 = float2_sub(&v3, &v1);
	const Float2 s2 = float2_sub(&v2, &v1);
	if (float2_cross(&s1, &s2) < 0) {
		Float2 tmp = v2;
		v2 = v3;
		v3 = tmp;
	}
	Float2 tmp = float2_sub(&v2, &v1);
	Ray2 r1 = ray2_new(&v1, &tmp);
	tmp = float2_sub(&v3, &v2);
	Ray2 r2 = ray2_new(&v2, &tmp);
	tmp = float2_sub(&v1, &v3);
	Ray2 r3 = ray2_new(&v3, &tmp);

	Triangle triangle;
	triangle.plane = plane;
	triangle.r1 = r1;
	triangle.r2 = r2;
	triangle.r3 = r3;
	return triangle;
}

int projection_type(const Plane* plane) {
	if (fabsf(plane->normal.x) >= fabsf(plane->normal.y) &&
		fabsf(plane->normal.x) >= fabsf(plane->normal.z))
		return YZ;
	if (fabsf(plane->normal.y) >= fabsf(plane->normal.x) &&
		fabsf(plane->normal.y) >= fabsf(plane->normal.z))
		return ZX;
	return XY;
}

float triangle_intersect_distance(const void* triangle, const Ray3* ray) {
	const Triangle* tri = (Triangle*)triangle;
	const float distance = plane_intersect_distance(&tri->plane, ray);
	if (distance < 0.0) return distance;
	const Float3 movement = float3_mul(&ray->direction, distance);
	const Float3 point = float3_add(&ray->origin, &movement);
	float cross1, cross2, cross3;
	const int type = projection_type(&tri->plane);
	Float2 projection;
	if (type == XY) projection = float2_new(point.x, point.y);
	else if (type == YZ) projection = float2_new(point.y, point.z);
	else projection = float2_new(point.z, point.x);
	Float2 tmp = float2_sub(&projection, &tri->r1.origin);
	cross1 = float2_cross(&tri->r1.direction, &tmp);
	tmp = float2_sub(&projection, &tri->r2.origin);
	cross2 = float2_cross(&tri->r2.direction, &tmp);
	tmp = float2_sub(&projection, &tri->r3.origin);
	cross3 = float2_cross(&tri->r3.direction, &tmp);
	if (cross1 < 0.0 && cross2 < 0.0 && cross3 < 0.0) return distance;
	return -1;
}

Float3 triangle_normal_vector(const void* triangle, const Float3* point) {
	Triangle* tri = (Triangle*)triangle;
	return plane_normal_vector(&tri->plane, point);
}
