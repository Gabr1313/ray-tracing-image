#pragma once
#include "algebra.h"
#include "camera.h"
#include "object.h"

typedef struct _InputData {
	int number_of_updates, max_bounces;
	Float3 background_color;
	Camera camera;
	char *color_ppm, *color_pfm, *albedo_pfm, *normal_pfm;
	ObjectVec objects;
} InputData;

InputData scan_input();
void free_input_data(InputData *input);
