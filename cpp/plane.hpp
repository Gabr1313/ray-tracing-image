#pragma once

#include "types.hpp"

// ax + by + cz + d = 0
class Plane {
   public:
	f32 a, b, c, d;
	Plane(f32 a, f32 b, f32 c, f32 d);
};
