#pragma once
#include "algebra.h"
#include "camera.h"
#include "object.h"

typedef struct _InputData {
	int number_of_updates, max_bounces, save_floats;
	Float3 background_color;
	Camera camera;
	ObjectVec objects;
} InputData;

InputData scan_input();
