#pragma once

#include "math/vec2.h"

#include <math.h>

namespace AnimMathUtils {

const float PI = 3.14159265359f;

vec2 calculateMaxScreenCoords(float fov, float aspect, float znear) {
	float ymax = znear * tanf(fov * 3.14159265359f / 360.0f);
	float xmax = ymax * aspect;

	return vec2(xmax * 1000, ymax * 1000);
}

}