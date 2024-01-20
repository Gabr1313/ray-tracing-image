#pragma once

#include "algebra.h"
#include "scanner.h"

void shoot_and_draw(const InputData* input_data);
Float3 trace_ray(const Ray3* ray, const ObjectVec* objects,
						const int max_bounces, const Float3* background);

inline Float3 trace_albedo(const Ray3* ray, const ObjectVec* objects,
						   const int max_bounces, const Float3* background);

inline Float3 trace_normal(const Ray3* ray, const ObjectVec* objects,
						   const int max_bounces, const Float3* background);
