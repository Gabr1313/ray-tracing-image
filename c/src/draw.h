#pragma once

#include "algebra.h"
#include "scanner.h"

void shoot_and_draw(const InputData* input_data);
void shoot_a_pixel(Float3* pixel_to_update, const int sqrt_ray_per_pixel,
				   const Ray3* upper_left, const Float3* d_x, const Float3* d_y,
				   const ObjectVec* objects, const float max_bounces,
				   const Float3* background,
				   Float3 (*trace_fn)(const Ray3*, const ObjectVec*, const int,
									  const Float3*));

Float3 trace_ray(const Ray3* ray, const ObjectVec* objects,
				 const int max_bounces, const Float3* background);

Float3 trace_albedo(const Ray3* ray, const ObjectVec* objects,
 				   const int max_bounces, const Float3* background);

Float3 trace_normal(const Ray3* ray, const ObjectVec* objects,
 				   const int max_bounces, const Float3* background);
Object* nearest_object(const Ray3* ray, const ObjectVec* objects,
							  const Object* prev, float* distance);

void translate_and_write_pfm(const char* filename, const InputData* input_data,
							 Float3* pixel_sum);
void calculate_and_write_pfm(
	const char* filename, const InputData* input_data, Float3* pixel_sum,
	Float3 (*trace_fn)(const Ray3*, const ObjectVec*, const int,
					   const Float3*));
void write_pfm(const char* filename, Float3* pixel_sum, const int width,
			   const int height);

