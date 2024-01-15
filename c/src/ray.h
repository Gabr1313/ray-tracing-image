#pragma once

#include "algebra.h"

typedef struct _Ray3 {
	Float3 origin, direction;
} Ray3;

Ray3 ray3_new(const Float3* position, const Float3* direction);
void ray3_move_along(Ray3* ray, const float distance);

typedef struct _Ray2 {
	Float2 origin, direction;
} Ray2;

Ray2 ray2_new(const Float2* origin, const Float2* direction);
