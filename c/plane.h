#pragma once

#include "algebra.h"
#include "ray.h"

typedef struct _Plane {
	Float3 normal;
	float d;
} Plane;

Plane plane_new(const float a, const float b, const float c, const float d);
Plane plane_from_points(const Float3* p1, const Float3* p2, const Float3* p3);
float plane_intersect_distance(const void* plane, const Ray3* ray);
Float3 plane_normal_vector(const void* plane, const Float3* point);
