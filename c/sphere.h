#pragma once

#include "algebra.h"
#include "ray.h"

typedef struct _Sphere {
	Float3 center;
	float radius;
} Sphere;

Sphere sphere_new(const Float3* center, const float radius);

float sphere_intersect_distance(const void* sphere, const Ray3* ray);
Float3 sphere_normal_vector(const void* sphere, const Float3* point);
