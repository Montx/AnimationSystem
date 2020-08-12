#pragma once

#include "render/Shader.h"
#include "render/Attribute.h"
#include "math/vec3.h"
#include "math/mat4.h"
#include "animation/Pose.h"

#include <vector>

enum class DebugDrawMode {
	Lines, Loop, Strip, Points
};

class DebugDraw {
public:
	DebugDraw(const DebugDraw&) = delete;
	DebugDraw& operator=(const DebugDraw&) = delete;

	DebugDraw();
	DebugDraw(unsigned int size);
	~DebugDraw();

	unsigned int Size();
	void Resize(unsigned int newSize);

	vec3& operator[](unsigned int index);
	void Push(const vec3& v);

	void FromPose(Pose& pose);

	void UpdateOpenGLBuffers();
	void Draw(DebugDrawMode mode, const vec3& color, const mat4& mvp);

protected:
	std::vector<vec3> mPoints;
	Attribute<vec3>* mAttribs;
	Shader* mShader;
};