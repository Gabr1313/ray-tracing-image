#include "camera.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "algebra.h"
#include "ray.h"

Float3* ul_ur_dl(Float3* direction, float angle, float ratio);

Camera camera_new(Float3* position, Float3* direction, float angle, int width,
				  int height, int sqrt_ray_per_pixel) {
	float ratio = (float)width / (float)height;
	Float3* corners_origin = ul_ur_dl(direction, angle, ratio);
	Camera cam;
	cam.upper_left = ray3_new(position, &corners_origin[0]);
	Float3 distance_x = float3_sub(&corners_origin[1], &corners_origin[0]);
	cam.delta_x = float3_div(&distance_x, (float)width);
	Float3 distance_y = float3_sub(&corners_origin[2], &corners_origin[0]);
	cam.delta_y = float3_div(&distance_y, (float)height);
	free(corners_origin);
	cam.width = width;
	cam.height = height;
	cam.sqrt_ray_per_pixel = sqrt_ray_per_pixel;
	cam.d_x = float3_div(&cam.delta_x, sqrt_ray_per_pixel);
	cam.d_y = float3_div(&cam.delta_y, sqrt_ray_per_pixel);
	return cam;
}

Float3* ul_ur_dl(Float3* direction, float angle, float ratio) {
	float alpha = angle / 2.0;
	Float3 point;
	Float3 x_axis = float3_new(1, 0, 0), z_axis = float3_new(0, 0, 1);

	if (ratio < 1.0) {
		Mat3 yaw = mat3_yaw(alpha);
		Float3 yawed = mat3_mul_float3(&yaw, &x_axis);
		point = float3_new(yawed.x, yawed.z * ratio, yawed.z);
	} else {
		Mat3 roll = mat3_roll(alpha);
		Float3 rolled = mat3_mul_float3(&roll, &x_axis);
		point = float3_new(rolled.x, rolled.y, rolled.y / ratio);
	}
	Float3 point_ul = float3_new(point.x, point.y, -point.z);
	Float3 point_ur = float3_new(point.x, -point.y, -point.z);
	Float3 point_dl = float3_new(point.x, point.y, point.z);
	if (direction->x == 0 && direction->y == 0) {
		fprintf(stderr,
				"Error: camera direction is not valid: x == 0 && y == 0\n");
		exit(-1);
	}
	Float3 dir1 = *direction;
	Float3 dir2 = float3_cross(&dir1, &z_axis);
	Float3 dir3 = float3_cross(&dir1, &dir2);
	float3_normalize_eq(&dir1);
	float3_normalize_eq(&dir2);
	float3_normalize_eq(&dir3);
	Mat3 change_of_basis = mat3_new(dir1.x, dir2.x, dir3.x, dir1.y, dir2.y,
									dir3.y, dir1.z, dir2.z, dir3.z);
	Float3 ul = mat3_mul_float3(&change_of_basis, &point_ul);
	Float3 ur = mat3_mul_float3(&change_of_basis, &point_ur);
	Float3 dl = mat3_mul_float3(&change_of_basis, &point_dl);
	Float3* retval = malloc(sizeof(Float3) * 3);
	if (retval == NULL) {
		fprintf(stderr, "Error: malloc failed\n");
		exit(-1);
	}
	memcpy(retval, &ul, sizeof(Float3));
	memcpy(retval + 1, &ur, sizeof(Float3));
	memcpy(retval + 2, &dl, sizeof(Float3));
	return retval;
}
