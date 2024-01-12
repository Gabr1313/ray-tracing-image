#include "algebra.h"

#include <math.h>
Float3 float3_new(float x, float y, float z) {
	Float3 f;
	f.x = x;
	f.y = y;
	f.z = z;
	return f;
}

float float3_dot(Float3* lhs, Float3* rhs) {
	return lhs->x * rhs->x + lhs->y * rhs->y + lhs->z * rhs->z;
}

Float3 float3_cross(Float3* lhs, Float3* rhs) {
	Float3 f;
	f.x = lhs->y * rhs->z - lhs->z * rhs->y;
	f.y = lhs->z * rhs->x - lhs->x * rhs->z;
	f.z = lhs->x * rhs->y - lhs->y * rhs->x;
	return f;
}

Float3 float3_add(Float3* lhs, Float3* rhs) {
	Float3 f;
	f.x = lhs->x + rhs->x;
	f.y = lhs->y + rhs->y;
	f.z = lhs->z + rhs->z;
	return f;
}

void float3_add_eq(Float3* lhs, Float3* rhs) {
	lhs->x += rhs->x;
	lhs->y += rhs->y;
	lhs->z += rhs->z;
}

Float3 float3_sub(Float3* lhs, Float3* rhs) {
	Float3 f;
	f.x = lhs->x - rhs->x;
	f.y = lhs->y - rhs->y;
	f.z = lhs->z - rhs->z;
	return f;
}

void float3_sub_eq(Float3* lhs, Float3* rhs) {
	lhs->x -= rhs->x;
	lhs->y -= rhs->y;
	lhs->z -= rhs->z;
}

Float3 float3_mul(Float3* lhs, float rhs) {
	Float3 f;
	f.x = lhs->x * rhs;
	f.y = lhs->y * rhs;
	f.z = lhs->z * rhs;
	return f;
}

Float3 float3_mul_float3(Float3* lhs, Float3* rhs) {
	Float3 f;
	f.x = lhs->x * rhs->x;
	f.y = lhs->y * rhs->y;
	f.z = lhs->z * rhs->z;
	return f;
}

void float3_mul_eq(Float3* lhs, float rhs) {
	lhs->x *= rhs;
	lhs->y *= rhs;
	lhs->z *= rhs;
}

Float3 float3_mul_eq_float3(Float3* lhs, Float3* rhs) {
	lhs->x *= rhs->x;
	lhs->y *= rhs->y;
	lhs->z *= rhs->z;
	return *lhs;
}

Float3 float3_div(Float3* lhs, float rhs) {
	float inv = 1.0 / rhs;
	return float3_mul(lhs, inv);
}

void float3_div_eq(Float3* lhs, float rhs) {
	float inv = 1.0 / rhs;
	float3_mul_eq(lhs, inv);
}

Float3 float3_normalize(Float3* v) {
	Float3 f = *v;
	float3_normalize_eq(&f);
	return f;
}

void float3_normalize_eq(Float3* v) {
	float norm = sqrtf(float3_dot(v, v));
	float inv = 1.0 / norm;
	v->x *= inv;
	v->y *= inv;
	v->z *= inv;
}

int float3_eq(Float3* lhs, Float3* rhs) {
	return lhs->x == rhs->x && lhs->y == rhs->y && lhs->z == rhs->z;
}

Float3 float3_mirror(Float3* lhs, Float3* rhs) {
	Float3 normalized = float3_normalize(rhs);
	float coeff = 2 * float3_dot(lhs, &normalized);
	Float3 sub = float3_mul(&normalized, coeff);
	return float3_sub(lhs, &sub);
}

void float3_invert_eq(Float3* lhs) {
	lhs->x = -lhs->x;
	lhs->y = -lhs->y;
	lhs->z = -lhs->z;
}

Float2 float2_new(float x, float y) {
	Float2 f;
	f.x = x;
	f.y = y;
	return f;
}

Float2 float2_sub(Float2* lhs, Float2* rhs) {
	Float2 f;
	f.x = lhs->x - rhs->x;
	f.y = lhs->y - rhs->y;
	return f;
}

float float2_dot(Float2* lhs, Float2* rhs) {
	return lhs->x * rhs->x + lhs->y * rhs->y;
}
float float2_cross(Float2* lhs, Float2* rhs) {
	return lhs->x * rhs->y - lhs->y * rhs->x;
}

Mat3 mat3_new(float m00, float m01, float m02, float m10, float m11, float m12,
			  float m20, float m21, float m22) {
	Mat3 mat;
	mat.m[0][0] = m00;
	mat.m[0][1] = m01;
	mat.m[0][2] = m02;
	mat.m[1][0] = m10;
	mat.m[1][1] = m11;
	mat.m[1][2] = m12;
	mat.m[2][0] = m20;
	mat.m[2][1] = m21;
	mat.m[2][2] = m22;
	return mat;
}

Mat3 mat3_yaw(float angle) {
	Mat3 mat;
	mat.m[0][0] = cosf(angle);
	mat.m[0][1] = 0;
	mat.m[0][2] = sinf(angle);
	mat.m[1][0] = 0;
	mat.m[1][1] = 1;
	mat.m[1][2] = 0;
	mat.m[2][0] = -sinf(angle);
	mat.m[2][1] = 0;
	mat.m[2][2] = cosf(angle);
	return mat;
}

Mat3 mat3_pitch(float angle) {
	Mat3 mat;
	mat.m[0][0] = 1;
	mat.m[0][1] = 0;
	mat.m[0][2] = 0;
	mat.m[1][0] = 0;
	mat.m[1][1] = cosf(angle);
	mat.m[1][2] = -sinf(angle);
	mat.m[2][0] = 0;
	mat.m[2][1] = sinf(angle);
	mat.m[2][2] = cosf(angle);
	return mat;
}

Mat3 mat3_roll(float angle) {
	Mat3 mat;
	mat.m[0][0] = cosf(angle);
	mat.m[0][1] = -sinf(angle);
	mat.m[0][2] = 0;
	mat.m[1][0] = sinf(angle);
	mat.m[1][1] = cosf(angle);
	mat.m[1][2] = 0;
	mat.m[2][0] = 0;
	mat.m[2][1] = 0;
	mat.m[2][2] = 1;
	return mat;
}

Float3 mat3_mul_float3(Mat3* lhs, Float3* rhs) {
	Float3 f;
	f.x = lhs->m[0][0] * rhs->x + lhs->m[0][1] * rhs->y + lhs->m[0][2] * rhs->z;
	f.y = lhs->m[1][0] * rhs->x + lhs->m[1][1] * rhs->y + lhs->m[1][2] * rhs->z;
	f.z = lhs->m[2][0] * rhs->x + lhs->m[2][1] * rhs->y + lhs->m[2][2] * rhs->z;
	return f;
}
