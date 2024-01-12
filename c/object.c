#include "object.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "algebra.h"

Object object_new(int shape_type, Shape* shape, Float3* color,
				  float emission_intensity, float reflection) {
	static int id = 0;
	Object object;
	object.id = id++;
	object.shape = *shape;
	object.color = *color;
	object.light_emitted = float3_mul(color, emission_intensity);
	object.reflection = reflection;
	object.shape_type = shape_type;
	if (shape_type == TYPE_SPHERE) {
		object.intersect_distance = sphere_intersect_distance;
		object.normal_vector = sphere_normal_vector;
	} else if (shape_type == TYPE_PLANE) {
		object.intersect_distance = plane_intersect_distance;
		object.normal_vector = plane_normal_vector;
	} else if (shape_type == TYPE_TRIANGLE) {
		object.intersect_distance = triangle_intersect_distance;
		object.normal_vector = triangle_normal_vector;
	} else {
		fprintf(stderr, "Error: unknown shape type %d\n", shape_type);
		exit(-1);
	}
	return object;
}

float object_intersect_distance(Object* object, Ray3* ray) {
	return object->intersect_distance(&object->shape, ray);
}

Float3 object_normal_vector(Object* object, Ray3* ray) {
	Float3 direction = object->normal_vector(&object->shape, &ray->origin);
	if (float3_dot(&ray->direction, &direction) > 0.0)
		float3_invert_eq(&direction);
	return direction;
}

ObjectVec objectvec_new(int n) {
	ObjectVec obj_container;
	obj_container.size = 0;
	obj_container.capacity = n;
	obj_container.ptr = malloc(sizeof(Object) * n);
	if (obj_container.ptr == NULL) {
		fprintf(stderr, "Error: malloc failed in new_object_container()\n");
		exit(-1);
	}
	return obj_container;
}

void object_vec_push(ObjectVec* object_v, Object* object) {
	if (object_v->size >= object_v->capacity) {
		fprintf(stderr, "Error: object_v->size >= object_v->capacity\n");
		exit(-1);
	}
	memcpy(object_v->ptr + object_v->size, object, sizeof(Object));
	object_v->size++;
}

void object_reflect(Object* object, Ray3* ray, float distance) {
	ray3_move_along(ray, distance);
	Float3 normal = object_normal_vector(object, ray);
	float flip = (float)rand() / RAND_MAX;
	if (flip < object->reflection) {
	ray->direction = float3_mirror(&ray->direction, &normal);
	} else {
		ray->direction = half_sphere_random(&normal);
	}
}

#define PI 3.14159265358979323846

Float3 half_sphere_random(Float3* normal) {
	float phi = 2 * PI * (float)rand() / RAND_MAX;
	float theta = PI * (float)rand() / RAND_MAX;
	float sin_theta = sinf(theta);
	Float3 retval = float3_new(sin_theta * cosf(phi), sin_theta * sinf(phi),
							   cosf(theta));
	if (float3_dot(&retval, normal) < 0) {
		float3_invert_eq(&retval);
	}
	return retval;
}
