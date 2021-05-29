#pragma once

#include "animation/Skeleton.h"
#include "animation/Pose.h"

#include "render/Attribute.h"
#include "render/IndexBuffer.h"

#include "math/mat4.h"
#include "math/vec4.h"
#include "math/vec3.h"
#include "math/vec2.h"

#include <vector>

class MeshFbx {
public:
	MeshFbx();
	MeshFbx(const MeshFbx&);
	MeshFbx& operator=(const MeshFbx&);
	~MeshFbx();

	std::vector<vec3>& GetPosition();
	std::vector<vec3>& GetNormal();
	std::vector<vec2>& GetTexCoord();
	std::vector<vec4>& GetWeights();
	std::vector<ivec4>& GetInfluences();
	std::vector<unsigned int>& GetIndices();

	void CPUSkin(Skeleton& skeleton, Pose& pose);
	void CPUSkin(std::vector<mat4>& animatedPose);
	void UpdateOpenGLBuffers();
	void Bind(int position, int normal, int texCoord, int weight, int influence);

	void Draw();
	void DrawInstanced(unsigned int numInstances);
	void UnBind(int position, int normal, int texCoord, int weigth, int influence);

protected:
	std::vector<vec3> mPosition;
	std::vector<vec3> mNormal;
	std::vector<vec2> mTexCoord;
	std::vector<vec4> mWeights;
	std::vector<ivec4> mInfluences;
	std::vector<unsigned int> mIndices;

	Attribute<vec3>* mPosAttrib;
	Attribute<vec3>* mNormAttrib;
	Attribute<vec2>* mUvAttrib;
	Attribute<vec4>* mWeightAttrib;
	Attribute<ivec4>* mInfluenceAttrib;
	IndexBuffer* mIndexBuffer;

	std::vector<vec3> mSkinnedPosition;
	std::vector<vec3> mSkinnedNormal;
	std::vector<mat4> mPosePalette;
};