#pragma once

#include "math/vec3.h"
#include "math/quat.h"

struct Transform {
	vec3 position;
	quat rotation;
	vec3 scale;

	inline Transform(const vec3& p, const quat& r, const vec3& s) 
		: position(p), rotation(r), scale(s) {}

	inline Transform() 
		: position(vec3(0, 0, 0))
		, rotation(quat(0, 0, 0, 1))
		, scale(vec3(1, 1, 1)) {}
};

Transform combine(const Transform& a, const Transform& b);
Transform inverse(const Transform& t);
Transform mix(const Transform& a, const Transform& b, float t);
bool operator==(const Transform& a, const Transform& b);
bool operator!=(const Transform& a, const Transform& b);
mat4 transformToMat4(const Transform& t);
Transform mat4ToTransform(const mat4& m);
vec3 transformPoint(const Transform& a, const vec3& b);
vec3 transformVector(const Transform& a, const vec3& b);

