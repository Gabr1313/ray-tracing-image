#pragma once

#include "algebra.hpp"

class Ray {
   public:
	Vec3 orig, dir;
	Ray(Vec3 orig, Vec3 dir);
};
