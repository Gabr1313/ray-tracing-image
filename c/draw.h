#pragma once

#include "algebra.h"
#include "scanner.h"

void shoot_and_draw(InputData* input_data, char* filename);
Float3 trace_ray(Ray3* ray, ObjectVec* objects, int max_bounces,
				 Float3* background);
