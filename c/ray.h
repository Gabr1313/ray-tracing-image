#pragma once

#include "algebra.h"

typedef struct _Ray3 {
	Float3 origin, direction;
} Ray3;

Ray3 ray3_new(Float3* position, Float3* direction);
void ray3_move_along(Ray3* ray, float distance);

typedef struct _Ray2 {
	Float2 origin, direction;
} Ray2;

Ray2 ray2_new(Float2* origin, Float2* direction);
