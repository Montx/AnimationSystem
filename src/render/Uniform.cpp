#include "render/Uniform.h"

#include "math/vec2.h"
#include "math/vec3.h"
#include "math/vec4.h"
#include "math/quat.h"
#include "math/mat4.h"

#include "glad.h"

#define UNIFORM_IMPL(gl_func, tType, dType) \
template<> void Uniform<tType>::Set(unsigned int slot, tType* data, unsigned int length) { \
gl_func(slot, (GLsizei)length, (dType*)&data[0]); \
}

template Uniform<int>;
template Uniform<float>;
template Uniform<ivec4>;
template Uniform<ivec2>;
template Uniform<vec2>;
template Uniform<vec3>;
template Uniform<vec4>;
template Uniform<quat>;
template Uniform<mat4>;

template<typename T>
void Uniform<T>::Set(unsigned int slot, const T& value) {
	Set(slot, (T*)&value, 1);
}

template <typename T>
void Uniform<T>::Set(unsigned int s, std::vector<T>& v) {
	Set(s, &v[0], (unsigned int)v.size());
}

UNIFORM_IMPL(glUniform1iv, int, int)

UNIFORM_IMPL(glUniform4iv, ivec4, int)

UNIFORM_IMPL(glUniform2iv, ivec2, int)

UNIFORM_IMPL(glUniform1fv, float, float)

UNIFORM_IMPL(glUniform2fv, vec2, float)

UNIFORM_IMPL(glUniform3fv, vec3, float)

UNIFORM_IMPL(glUniform4fv, vec4, float)

UNIFORM_IMPL(glUniform4fv, quat, float)

template<> 
void Uniform<mat4>::Set(unsigned int slot, mat4* inputArray, unsigned int arrayLength) {
	glUniformMatrix4fv(slot, (GLsizei)arrayLength, false, (float*)&inputArray[0]);
}