#include "data.h"

void Camera::set_location (float x, float y, float z) {
	location.x = x;
	location.y = y;
	location.z = z;
}

void Camera::set_up (float x, float y, float z) {
	up.x = x;
	up.y = y;
	up.z = z;
}

void Camera::set_right (float x, float y, float z) {
	right.x = x;
	right.y = y;
	right.z = z;
}

void Camera::set_lookat (float x, float y, float z) {
	lookat.x = x;
	lookat.y = y;
	lookat.z = z;
}

void Light::set_location (float x, float y, float z) {
	location.x = x;
	location.y = y;
	location.z = z;
}

void Light::set_color (float x, float y, float z) {
	color.x = x;
	color.y = y;
	color.z = z;
}

void Sphere::set_center (float x, float y, float z) {
	center.x = x;
	center.y = y;
	center.z = z;
}

void Sphere::set_radius (float r) {
	radius = r;
}

void Sphere::set_color (float r, float g, float b) {
	color.x = r;
	color.y = g;
	color.z = b;
}

void Sphere::set_ambient (float x) {
	ambient = x;
}

void Sphere::set_diffuse (float x) {
	diffuse = x;
}

void Sphere::set_translate(float x, float y, float z) {
	translate.x = x;
	translate.y = y;
	translate.z = z;
}

void Plane::set_normal(float x, float y, float z) {
	normal.x = x;
	normal.y = y;
	normal.z = z;
}

void Plane::set_distance(float r) {
	distance = r;
}

void Plane::set_color(float r, float g, float b) {
	color.x = r;
	color.y = g;
	color.z = b;
}

void Plane::set_ambient(float x) {
	ambient = x;
}

void Plane::set_diffuse(float x) {
	diffuse = x;
}

void Triangle::set_p1(float x, float y, float z) {
	p1.x = x;
	p1.y = y;
	p1.z = z;
}

void Triangle::set_p2(float x, float y, float z) {
	p2.x = x;
	p2.y = y;
	p2.z = z;
}

void Triangle::set_p3(float x, float y, float z) {
	p3.x = x;
	p3.y = y;
	p3.z = z;
}

void Triangle::set_color(float r, float g, float b) {
	color.x = r;
	color.y = g;
	color.z = b;
}
