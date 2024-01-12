#include "scanner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "algebra.h"
#include "camera.h"
#include "object.h"

void next_valid_word(char* buffer, FILE* file);
int next_int(char* buffer, FILE* file);
float next_float(char* buffer, FILE* file);
Float3 next_float3(char* buffer, FILE* file);

InputData scan_input(char* input) {
	FILE* file;
	file = fopen(input, "r");
	if (file == NULL) {
		fprintf(stderr, "Error: can't open file %s\n", input);
		exit(-1);
	}
	char buffer[256];
	InputData input_data;
	int width = next_int(buffer, file);
	int height = next_int(buffer, file);
	next_int(buffer, file);	 // n_threads
	int sqrt_ray_per_pixel = next_int(buffer, file);
	input_data.number_of_updates = next_int(buffer, file);
	Float3 camera_position = next_float3(buffer, file);
	Float3 camera_direction = next_float3(buffer, file);
	float camera_angle = next_float(buffer, file);
	input_data.camera =
		camera_new(&camera_position, &camera_direction, camera_angle, width,
				   height, sqrt_ray_per_pixel);
	input_data.max_bounces = next_int(buffer, file);
	input_data.background_color = next_float3(buffer, file);

	int n_objects = next_int(buffer, file);
	input_data.objects = objectvec_new(n_objects);

	for (int i = 0; i < n_objects; i++) {
		next_valid_word(buffer, file);
		int shape_type;
		Shape shape;

		if (strcmp(buffer, "sphere") == 0) {
			Float3 center = next_float3(buffer, file);
			float radius = next_float(buffer, file);
			shape_type = TYPE_SPHERE;
			shape.sphere = sphere_new(&center, radius);
		} else if (strcmp(buffer, "plane") == 0) {
			float a = next_float(buffer, file);
			float b = next_float(buffer, file);
			float c = next_float(buffer, file);
			float d = next_float(buffer, file);
			shape_type = TYPE_PLANE;
			shape.plane = plane_new(a, b, c, d);
		} else if (strcmp(buffer, "triangle") == 0) {
			Float3 point_1 = next_float3(buffer, file);
			Float3 point_2 = next_float3(buffer, file);
			Float3 point_3 = next_float3(buffer, file);
			shape_type = TYPE_TRIANGLE;
			shape.triangle = triangle_new(&point_1, &point_2, &point_3);
		} else {
			fprintf(stderr, "Parse error: unknown object %s", buffer);
			exit(-1);
		}
		Float3 color = next_float3(buffer, file);
		float reflection = next_float(buffer, file);
		float emission_intensity = next_float(buffer, file);
		Object object = object_new(shape_type, &shape, &color,
								   emission_intensity, reflection);
		object_vec_push(&input_data.objects, &object);
	}
	fclose(file);
	return input_data;
}

void next_valid_word(char* buffer, FILE* file) {
	do {
		if (fscanf(file, "%s", buffer) == 0) {
			fprintf(stderr, "fscanf can't read any more data");
			exit(-1);
		}
	} while (buffer[0] == '_');
}

int next_int(char* buffer, FILE* file) {
	int tmp;
	next_valid_word(buffer, file);
	sscanf(buffer, "%d", &tmp);
	return tmp;
}

float next_float(char* buffer, FILE* file) {
	float tmp;
	next_valid_word(buffer, file);
	sscanf(buffer, "%f", &tmp);
	return tmp;
}

Float3 next_float3(char* buffer, FILE* file) {
	float x = next_float(buffer, file);
	float y = next_float(buffer, file);
	float z = next_float(buffer, file);
	return float3_new(x, y, z);
}
