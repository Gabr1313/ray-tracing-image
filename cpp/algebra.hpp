#pragma once
#include <array>
#include <initializer_list>

#include "types.hpp"

class Vec3 {
   public:
	f32 x, y, z;
	Vec3(f32 x, f32 y, f32 z);
	Vec3(std::initializer_list<f32> values);
	Vec3 crossed(Vec3 &v);
	Vec3 normalized();
};

class Mat3 {
   public:
	std::array<std::array<f32, 3>, 3> m;
	Mat3(std::array<std::array<f32, 3>, 3> m);
	static Mat3 yaw(f32 theta);
	static Mat3 pitch(f32 theta);
	static Mat3 roll(f32 theta);
	Vec3 operator*(Vec3 &v);
};
