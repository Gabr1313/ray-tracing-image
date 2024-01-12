#pragma once

#include "algebra.h"
#include "ray.h"

typedef struct _Plane {
	Float3 normal;
	float d;
} Plane;

Plane plane_new(float a, float b, float c, float d);
Plane plane_from_points(Float3* p1, Float3* p2, Float3* p3);
float plane_intersect_distance(void* plane, Ray3* ray);
Float3 plane_normal_vector(void* plane, Float3* point);
