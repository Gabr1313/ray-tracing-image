#pragma once

#include "algebra.h"
#include "plane.h"
#include "ray.h"

typedef struct _Triangle {
	Plane plane;
	Ray2 r1, r2, r3;
} Triangle;

Triangle triangle_new(Float3* p1, Float3* p2, Float3* p3);
float triangle_intersect_distance(void* triangle, Ray3* ray);
Float3 triangle_normal_vector(void* triangle, Float3* point);
