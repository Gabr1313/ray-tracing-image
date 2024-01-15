#pragma once

typedef struct _Float3 {
	float x, y, z;
} Float3;

Float3 float3_new(const float x, const float y, const float z);
float float3_dot(const Float3* lhs, const Float3* rhs);
Float3 float3_cross(const Float3* lhs, const Float3* rhs);
float float3_length(const Float3* v);
Float3 float3_normalize(const Float3* v);
void float3_normalize_eq(Float3* v);
Float3 float3_add(const Float3* lhs, const Float3* rhs);
void float3_add_eq(Float3* lhs, const Float3* rhs);
Float3 float3_sub(const Float3* lhs, const Float3* rhs);
void float3_sub_eq(Float3* lhs, const Float3* rhs);
Float3 float3_mul(const Float3* lhs, const float rhs);
void float3_mul_eq(Float3* lhs, const float rhs);
void float3_mul_eq_float3(Float3* lhs, const Float3* rhs);
Float3 float3_mul_float3(const Float3* lhs, const Float3* rhs);
Float3 float3_div(const Float3* lhs, const float rhs);
void float3_div_eq(Float3* lhs, const float rhs);
int float3_eq(const Float3* lhs, const Float3* rhs);
Float3 float3_mirror(const Float3* lhs, const Float3* rhs);
void float3_invert_eq(Float3* lhs);

typedef struct _Float2 {
	float x, y;
} Float2;

Float2 float2_new(const float x, const float y);
Float2 float2_sub(const Float2* lhs, const Float2* rhs);
float float2_dot(const Float2* lhs, const Float2* rhs);
float float2_cross(const Float2* lhs, const Float2* rhs);

typedef struct _Mat3 {
	float m[3][3];
} Mat3;

Mat3 mat3_new(const float m00, const float m01, const float m02,
			  const float m10, const float m11, const float m12,
			  const float m20, const float m21, const float m22);
Mat3 mat3_yaw(const float angle);
Mat3 mat3_pitch(const float angle);
Mat3 mat3_roll(const float angle);
Float3 mat3_mul_float3(const Mat3* lhs, const Float3* rhs);
