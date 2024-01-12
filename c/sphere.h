#pragma once

#include "algebra.h"
#include "ray.h"

typedef struct _Sphere {
	Float3 center;
	float radius;
} Sphere;

Sphere sphere_new(Float3* center, float radius);

float sphere_intersect_distance(void* sphere, Ray3* ray);
Float3 sphere_normal_vector(void* sphere, Float3* point);
