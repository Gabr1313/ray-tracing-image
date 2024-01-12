#include "algebra.hpp"

#include <cmath>
#include <initializer_list>

Vec3::Vec3(f32 x, f32 y, f32 z) : x(x), y(y), z(z) {}

Vec3::Vec3(std::initializer_list<f32> values) {
	auto begin = values.begin();
	x = begin[0];
	y = begin[1];
	z = begin[2];
}

Vec3 Vec3::crossed(Vec3& v) {
	Vec3 r = {y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x}; 
	return r;
}
 
Vec3 Vec3::normalized() {
	f32 norm = sqrtf(x * x + y * y + z * z);
	Vec3 r = {x / norm, y / norm, z / norm};
	return r;
}

Mat3::Mat3(std::array<std::array<f32, 3>, 3> m) : m(m) {}

Mat3 Mat3::yaw(f32 theta) {
	Mat3 m({{{cosf(theta), 0, sinf(theta)},
			 {0, 1, 0},
			 {-sinf(theta), 0, cosf(theta)}}});
	return m;
}

Mat3 Mat3::pitch(f32 theta) {
	Mat3 m({{{1, 0, 0},
			 {0, cosf(theta), -sinf(theta)},
			 {0, sinf(theta), cosf(theta)}}});
	return m;
}

Mat3 Mat3::roll(f32 theta) {
	Mat3 m({{{cosf(theta), -sinf(theta), 0},
			 {sinf(theta), cosf(theta), 0},
			 {0, 0, 1}}});
	return m;
}

Vec3 Mat3::operator*(Vec3& v) {
	auto pino = v.x;
	Vec3 r = {m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
			  m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
			  m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z};
	return r;
}
