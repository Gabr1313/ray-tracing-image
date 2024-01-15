#include "draw.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "algebra.h"
#include "object.h"
#include "ray.h"

void shoot_a_pixel(Float3* pixel_to_update, const int sqrt_ray_per_pixel,
				   const Ray3* upper_left, const Float3* d_x, const Float3* d_y,
				   const ObjectVec* objects, const float max_bounces,
				   const Float3* background);

int min(int a, int b) { return a < b ? a : b; }

void shoot_and_draw(const InputData* input_data) {
	char header[64];

	const int width = input_data->camera.width;
	const int height = input_data->camera.height;
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

	const int content_len = width * height * 3;
	Float3* pixel_sum = calloc(width * height, sizeof(Float3));
	unsigned char* buffer = malloc(content_len * sizeof(unsigned char));
	if (pixel_sum == NULL) {
		fprintf(stderr, "Error: can't allocate memory for %d pixel\n",
				width * height);
		exit(-1);
	}

	sprintf(header, "P6\n%d %d\n255\n", width, height);
	const int heder_len = strlen(header);

	fwrite(header, sizeof(char), heder_len, stdout);

	srand(time(NULL));
	fprintf(stderr, "0 / %d", number_of_updates);
	for (int nou = 1; nou <= number_of_updates; nou++) {
		const float to_multiply = 255.0f / (nou * ray_per_pixel);
		Ray3 row = *upper_left;
		for (int i = 0, idx = 0, idx_buffer = 0; i < height; i++) {
			Ray3 col = row;
			for (int j = 0; j < width; j++, idx++) {
				shoot_a_pixel(&pixel_sum[idx], sqrt_ray_per_pixel, &col, d_x,
							  d_y, objects, max_bounces, background);
				buffer[idx_buffer++] = min(pixel_sum[idx].x * to_multiply, 255);
				buffer[idx_buffer++] = min(pixel_sum[idx].y * to_multiply, 255);
				buffer[idx_buffer++] = min(pixel_sum[idx].z * to_multiply, 255);
				float3_add_eq(&col.direction, delta_x);
			}
			float3_add_eq(&row.direction, delta_y);
		}
		fseek(stdout, heder_len, SEEK_SET);
		fwrite(buffer, sizeof(unsigned char), content_len, stdout);
		fflush(stdout);
		fprintf(stderr, "\r%d / %d", nou, number_of_updates);
	}
	fprintf(stderr, "\n");
	free(buffer);
	free(pixel_sum);
	free(input_data->objects.ptr);
}

inline void shoot_a_pixel(Float3* pixel_to_update, const int sqrt_ray_per_pixel,
						  const Ray3* upper_left, const Float3* d_x,
						  const Float3* d_y, const ObjectVec* objects,
						  const float max_bounces, const Float3* background) {
	Ray3 ray = *upper_left;
	Float3 starting_direction = ray.direction;
	for (int ii = 0; ii < sqrt_ray_per_pixel; ii++) {
		ray.direction = starting_direction;
		for (int jj = 0; jj < sqrt_ray_per_pixel; jj++) {
			Float3 light = trace_ray(&ray, objects, max_bounces, background);
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
		Object* nearest_object = NULL;
		float nearest_distance = INFINITY;
		for (int j = 0; j < objects->size; j++) {
			Object* object_found = &objects->ptr[j];
			float distance_found =
				object_intersect_distance(object_found, &local_ray);
			if (distance_found > 0 && prev != object_found &&
				distance_found < nearest_distance) {
				nearest_object = object_found;
				nearest_distance = distance_found;
			}
		}
		if (nearest_object == NULL) {
			const Float3 added_light = float3_mul_float3(background, &color);
			float3_add_eq(&light, &added_light);
			break;
		} else {
			prev = nearest_object;
			object_reflect_ray(nearest_object, &local_ray, nearest_distance);
			const Float3 added_light =
				float3_mul_float3(&nearest_object->light_emitted, &color);
			float3_add_eq(&light, &added_light);
			float3_mul_eq_float3(&color, &nearest_object->color);
		}
	}
	return light;
}