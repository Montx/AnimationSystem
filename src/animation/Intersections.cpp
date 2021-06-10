#include "animation/Intersections.h"

bool RaycastTriangle(const Ray& ray, const Triangle& triangle, vec3& hitPoint) {
	const float EPSILON = 0.0000001f;

	vec3 vertex0 = triangle.v0;
	vec3 vertex1 = triangle.v1;
	vec3 vertex2 = triangle.v2;

	vec3 edge1, edge2, h, s, q;

	vec3 rayVector = ray.direction;
	vec3 rayOrigin = ray.origin;

	float a, f, u, v;
	edge1 = vertex1 - vertex0;
	edge2 = vertex2 - vertex0;

	h = cross(rayVector, edge2);

}