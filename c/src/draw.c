#include "draw.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "algebra.h"
#include "object.h"
#include "ray.h"
#include "scanner.h"

int int_min(int a, int b) { return a < b ? a : b; }

void shoot_and_draw(const InputData* input_data) {
	char header[64];
	const int width = input_data->camera.width;
	const int height = input_data->camera.height;
	const int total_pixel = width * height;
	const int sqrt_ray_per_pixel = input_data->camera.sqrt_ray_per_pixel;
	const int ray_per_pixel = sqrt_ray_per_pixel * sqrt_ray_per_pixel;
	const int number_of_updates = input_data->number_of_updates;
	const int max_bounces = input_data->max_bounces;
	const Ray3* upper_left = &input_data->camera.upper_left;
	const Float3* delta_x = &input_data->camera.delta_x;
	const Float3* delta_y = &input_data->camera.delta_y;
	const Float3* d_x = &input_data->camera.d_x;
	const Float3* d_y = &input_data->camera.d_y;
	const Float3* background = &input_data->background_color;
	const ObjectVec* objects = &input_data->objects;

	const int content_len = total_pixel * 3;
	Float3* pixel_sum = calloc(total_pixel, sizeof(Float3));
	unsigned char* buffer = malloc(content_len * sizeof(unsigned char));
	if (pixel_sum == NULL) {
		fprintf(stderr, "Error: can't allocate memory for %d pixel\n",
				total_pixel);
		exit(-1);
	}

	const char* color_ppm = input_data->color_ppm;
	sprintf(header, "P6\n%d %d\n255\n", width, height);
	const int heder_len = strlen(header);
	FILE* file = fopen(color_ppm, "wb");
	fwrite(header, sizeof(char), heder_len, file);

	srand(time(NULL));
	fprintf(stderr, "ray tracing: 0 / %d", number_of_updates);
	for (int nou = 1; nou <= number_of_updates; nou++) {
		const float to_multiply = 255.0f / (nou * ray_per_pixel);
		Ray3 row = *upper_left;
		for (int i = 0, idx = 0, idx_buffer = 0; i < height; i++) {
			Ray3 col = row;
			for (int j = 0; j < width; j++, idx++) {
				shoot_a_pixel(&pixel_sum[idx], sqrt_ray_per_pixel, &col, d_x,
							  d_y, objects, max_bounces, background, trace_ray);
				buffer[idx_buffer++] =
					int_min(pixel_sum[idx].x * to_multiply, 255);
				buffer[idx_buffer++] =
					int_min(pixel_sum[idx].y * to_multiply, 255);
				buffer[idx_buffer++] =
					int_min(pixel_sum[idx].z * to_multiply, 255);
				float3_add_eq(&col.direction, delta_x);
			}
			float3_add_eq(&row.direction, delta_y);
		}

		fseek(file, heder_len, SEEK_SET);
		fwrite(buffer, sizeof(unsigned char), content_len, file);
		fflush(file);
		fprintf(stderr, "\rray tracing: %d / %d", nou, number_of_updates);
	}
	fclose(file);
	fprintf(stderr, "\n");

	const char* color_pfm = input_data->color_pfm;
	const char* albedo_pfm = input_data->albedo_pfm;
	const char* normal_pfm = input_data->normal_pfm;
	if (color_pfm != NULL && albedo_pfm != NULL && normal_pfm != NULL) {
		translate_and_write_pfm(color_pfm, input_data, pixel_sum);
		calculate_and_write_pfm(albedo_pfm, input_data, pixel_sum,
								trace_albedo);
		calculate_and_write_pfm(normal_pfm, input_data, pixel_sum,
								trace_normal);
	}

	free(buffer);
	free(pixel_sum);
}

inline void shoot_a_pixel(Float3* pixel_to_update, const int sqrt_ray_per_pixel,
						  const Ray3* upper_left, const Float3* d_x,
						  const Float3* d_y, const ObjectVec* objects,
						  const float max_bounces, const Float3* background,
						  Float3 (*trace_fn)(const Ray3*, const ObjectVec*,
											 const int, const Float3*)) {
	Ray3 ray = *upper_left;
	Float3 starting_direction = ray.direction;
	for (int ii = 0; ii < sqrt_ray_per_pixel; ii++) {
		ray.direction = starting_direction;
		for (int jj = 0; jj < sqrt_ray_per_pixel; jj++) {
			Float3 light = trace_fn(&ray, objects, max_bounces, background);
			float3_add_eq(pixel_to_update, &light);
			float3_add_eq(&ray.direction, d_x);
		}
		float3_add_eq(&starting_direction, d_y);
	}
}

inline Float3 trace_ray(const Ray3* ray, const ObjectVec* objects,
						const int max_bounces, const Float3* background) {
	Float3 color = float3_new(1, 1, 1);
	Float3 light = float3_new(0, 0, 0);
	Ray3 local_ray = *ray;
	Object* prev = NULL;
	for (int i = 0; i < max_bounces; i++) {
		float distance;
		Object* obj = nearest_object(&local_ray, objects, prev, &distance);
		if (obj == NULL) {
			const Float3 added_light = float3_mul_float3(background, &color);
			float3_add_eq(&light, &added_light);
			break;
		} else {
			prev = obj;
			object_reflect_ray(obj, &local_ray, distance);
			const Float3 added_light =
				float3_mul_float3(&obj->light_emitted, &color);
			float3_add_eq(&light, &added_light);
			float3_mul_eq_float3(&color, &obj->color);
		}
	}
	return light;
}

inline Float3 trace_albedo(const Ray3* ray, const ObjectVec* objects,
						   __attribute__((unused)) const int max_bounces,
						   __attribute__((unused)) const Float3* background) {
	Object* obj = nearest_object(ray, objects, NULL, NULL);
	if (obj != NULL) {
		return obj->color;
	}
	return float3_new(0, 0, 0);
}

inline Float3 trace_normal(const Ray3* ray, const ObjectVec* objects,
						   __attribute__((unused)) const int max_bounces,
						   __attribute__((unused)) const Float3* background) {
	Ray3 local_ray = *ray;
	float distance;
	Object* obj = nearest_object(&local_ray, objects, NULL, &distance);
	if (obj != NULL) {
		ray3_move_along(&local_ray, distance);
		return object_normal_normalized(obj, &local_ray);
	}
	return float3_new(0, 0, 0);
}

inline Object* nearest_object(const Ray3* ray, const ObjectVec* objects,
							  const Object* prev, float* distance) {
	Object* nearest_object = NULL;
	float nearest_distance = INFINITY;
	for (int j = 0; j < objects->size; j++) {
		Object* object_found = &objects->ptr[j];
		float distance_found = object_intersect_distance(object_found, ray);
		if (distance_found > 0 && prev != object_found &&
			distance_found < nearest_distance) {
			nearest_object = object_found;
			nearest_distance = distance_found;
		}
	}
	if (distance != NULL) *distance = nearest_distance;
	return nearest_object;
}

inline void translate_and_write_pfm(const char* filename,
									const InputData* input_data,
									Float3* pixel_sum) {
	const int height = input_data->camera.height;
	const int width = input_data->camera.width;
	const int total_pixel = width * height;
	const int sqrt_ray_per_pixel = input_data->camera.sqrt_ray_per_pixel;
	const int ray_per_pixel = sqrt_ray_per_pixel * sqrt_ray_per_pixel;
	const int number_of_updates = input_data->number_of_updates;

	fprintf(stderr, "%s: 0 / 1", filename);
	float to_multiply = 1.0f / (number_of_updates * ray_per_pixel);
	for (int i = 0; i < total_pixel; i++)
		float3_mul_eq(&pixel_sum[i], to_multiply);
	write_pfm(filename, pixel_sum, width, height);
	fprintf(stderr, "\r%s: 1 / 1\n", filename);
}

inline void calculate_and_write_pfm(
	const char* filename, const InputData* input_data, Float3* pixel_sum,
	Float3 (*trace_fn)(const Ray3*, const ObjectVec*, const int,
					   const Float3*)) {
	const int height = input_data->camera.height;
	const int width = input_data->camera.width;
	const int sqrt_ray_per_pixel = input_data->camera.sqrt_ray_per_pixel;
	const int max_bounces = input_data->max_bounces;
	const Float3* delta_x = &input_data->camera.delta_x;
	const Float3* delta_y = &input_data->camera.delta_y;
	const Float3* d_x = &input_data->camera.d_x;
	const Float3* d_y = &input_data->camera.d_y;
	const ObjectVec* objects = &input_data->objects;
	const int ray_per_pixel = input_data->camera.sqrt_ray_per_pixel *
							  input_data->camera.sqrt_ray_per_pixel;
	const Float3* background = &input_data->background_color;

	Ray3 row = input_data->camera.upper_left;
	fprintf(stderr, "%s: 0 / 1", filename);
	float to_multiply = 1.0f / ray_per_pixel;
	for (int i = 0, idx = 0; i < height; i++) {
		Ray3 col = row;
		for (int j = 0; j < width; j++, idx++) {
			pixel_sum[idx] = float3_new(0, 0, 0);
			shoot_a_pixel(&pixel_sum[idx], sqrt_ray_per_pixel, &col, d_x, d_y,
						  objects, max_bounces, background, trace_fn);
			float3_mul_eq(&pixel_sum[idx], to_multiply);
			float3_add_eq(&col.direction, delta_x);
		}
		float3_add_eq(&row.direction, delta_y);
	}
	write_pfm(filename, pixel_sum, width, height);
	fprintf(stderr, "\r%s: 1 / 1\n", filename);
}

inline void write_pfm(const char* filename, Float3* pixel_sum, const int width,
					  const int height) {
	const int total_pixel = width * height;

	FILE* file = fopen(filename, "wb");
	if (file == NULL) {
		fprintf(stderr, "Error: can't open file %s\n", filename);
		exit(-1);
	}
	fprintf(file, "PF\n%d %d\n-1.0\n", width, height);
	fwrite(pixel_sum, sizeof(Float3), total_pixel, file);
	fclose(file);
}
