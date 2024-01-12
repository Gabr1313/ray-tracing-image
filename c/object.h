#pragma once

#include "algebra.h"
#include "plane.h"
#include "ray.h"
#include "sphere.h"
#include "triangle.h"

#define TYPE_SPHERE 1
#define TYPE_PLANE 2
#define TYPE_TRIANGLE 3

typedef union _Shape {
	Sphere sphere;
	Plane plane;
	Triangle triangle;
} Shape;

typedef struct _Object {
	int shape_type, id;
	Shape shape;
	Float3 color, light_emitted;
	float reflection;
	float (*intersect_distance)(void* obj, Ray3* ray);
	Float3 (*normal_vector)(void* obj, Float3* point);
} Object;

Object object_new(int shape_id, Shape* shape, Float3* color,
				  float emission_intensity, float reflection);
float object_intersect_distance(Object* object, Ray3* ray);
Float3 object_normal_vector(Object* object, Ray3* ray);
void object_reflect(Object* object, Ray3* ray, float distance);
Float3 half_sphere_random(Float3*);

typedef struct _ObjectContainer {
	Object* ptr;
	int size, capacity;
} ObjectVec;

ObjectVec objectvec_new(int n);
void object_vec_push(ObjectVec* object_v, Object* object);
