#pragma once

typedef struct _Float3 {
	float x, y, z;
} Float3;

Float3 float3_new(float x, float y, float z);
float float3_dot(Float3* lhs, Float3* rhs);
Float3 float3_cross(Float3* lhs, Float3* rhs);
Float3 float3_normalize(Float3* v);
void float3_normalize_eq(Float3* v);
Float3 float3_add(Float3* lhs, Float3* rhs);
void float3_add_eq(Float3* lhs, Float3* rhs);
Float3 float3_sub(Float3* lhs, Float3* rhs);
void float3_sub_eq(Float3* lhs, Float3* rhs);
Float3 float3_mul(Float3* lhs, float rhs);
void float3_mul_eq(Float3* lhs, float rhs);
Float3 float3_mul_eq_float3(Float3* lhs, Float3* rhs);
Float3 float3_mul_float3(Float3* lhs, Float3* rhs);
Float3 float3_div(Float3* lhs, float rhs);
void float3_div_eq(Float3* lhs, float rhs);
int float3_eq(Float3* lhs, Float3* rhs);
Float3 float3_mirror(Float3* lhs, Float3* rhs);
void float3_invert_eq(Float3* lhs);

typedef struct _Float2 {
	float x, y;
} Float2;

Float2 float2_new(float x, float y);
Float2 float2_sub(Float2* lhs, Float2* rhs);
float float2_dot(Float2* lhs, Float2* rhs);
float float2_cross(Float2* lhs, Float2* rhs);

typedef struct _Mat3 {
	float m[3][3];
} Mat3;

Mat3 mat3_new(float m00, float m01, float m02, float m10, float m11, float m12,
			  float m20, float m21, float m22);
Mat3 mat3_yaw(float angle);
Mat3 mat3_pitch(float angle);
Mat3 mat3_roll(float angle);
Float3 mat3_mul_float3(Mat3* lhs, Float3* rhs);
