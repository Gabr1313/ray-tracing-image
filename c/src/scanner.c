#include "scanner.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "algebra.h"
#include "camera.h"
#include "object.h"

void next_valid_word(char* buffer);
int next_int(char* buffer);
float next_float(char* buffer);
Float3 next_float3(char* buffer);

InputData scan_input() {
	char buffer[256];
	InputData input_data;
	const int width = next_int(buffer);
	const int height = next_int(buffer);
	input_data.save_floats = next_int(buffer);
	const int sqrt_ray_per_pixel = next_int(buffer);
	input_data.number_of_updates = next_int(buffer);
	const Float3 camera_position = next_float3(buffer);
	const Float3 camera_direction = next_float3(buffer);
	const float camera_angle = next_float(buffer);
	input_data.camera =
		camera_new(&camera_position, &camera_direction, camera_angle, width,
				   height, sqrt_ray_per_pixel);
	input_data.max_bounces = next_int(buffer);
	input_data.background_color = next_float3(buffer);

	const int n_objects = next_int(buffer);
	input_data.objects = objectvec_new(n_objects);

	for (int i = 0; i < n_objects; i++) {
		next_valid_word(buffer);
		int shape_type;
		Shape shape;
		if (strcmp(buffer, "sphere") == 0) {
			Float3 center = next_float3(buffer);
			float radius = next_float(buffer);
			shape_type = TYPE_SPHERE;
			shape.sphere = sphere_new(&center, radius);
		} else if (strcmp(buffer, "plane") == 0) {
			float a = next_float(buffer);
			float b = next_float(buffer);
			float c = next_float(buffer);
			float d = next_float(buffer);
			shape_type = TYPE_PLANE;
			shape.plane = plane_new(a, b, c, d);
		} else if (strcmp(buffer, "triangle") == 0) {
			Float3 point_1 = next_float3(buffer);
			Float3 point_2 = next_float3(buffer);
			Float3 point_3 = next_float3(buffer);
			shape_type = TYPE_TRIANGLE;
			shape.triangle = triangle_new(&point_1, &point_2, &point_3);
		} else {
			fprintf(stderr, "Parse error: unknown object %s", buffer);
			exit(-1);
		}
		const Float3 color = next_float3(buffer);
		const float reflection = next_float(buffer);
		const float emission_intensity = next_float(buffer);
		const Object object = object_new(shape_type, &shape, &color,
										 emission_intensity, reflection);
		object_vec_push(&input_data.objects, &object);
	}
	return input_data;
}

void next_valid_word(char* buffer) {
	do {
		if (scanf("%s", buffer) == 0) {
			fprintf(stderr, "fscanf can't read any more data");
			exit(-1);
		}
	} while (buffer[0] == '_');
}

int next_int(char* buffer) {
	int tmp;
	next_valid_word(buffer);
	sscanf(buffer, "%d", &tmp);
	return tmp;
}

float next_float(char* buffer) {
	float tmp;
	next_valid_word(buffer);
	sscanf(buffer, "%f", &tmp);
	return tmp;
}

Float3 next_float3(char* buffer) {
	const float x = next_float(buffer);
	const float y = next_float(buffer);
	const float z = next_float(buffer);
	return float3_new(x, y, z);
}
