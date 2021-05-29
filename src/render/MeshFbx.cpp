#include "render/MeshFbx.h"

#include "render/Draw.h"

MeshFbx::MeshFbx()
	: mPosAttrib(new Attribute<vec3>())
	, mNormAttrib(new Attribute<vec3>())
	, mUvAttrib(new Attribute<vec2>())
	, mWeightAttrib(new Attribute<vec4>())
	, mInfluenceAttrib(new Attribute<ivec4>())
	, mIndexBuffer(new IndexBuffer()) {}

MeshFbx::MeshFbx(const MeshFbx& other)
	: mPosAttrib(new Attribute<vec3>())
	, mNormAttrib(new Attribute<vec3>())
	, mUvAttrib(new Attribute<vec2>())
	, mWeightAttrib(new Attribute<vec4>())
	, mInfluenceAttrib(new Attribute<ivec4>())
	, mIndexBuffer(new IndexBuffer()) {

	*this = other;
}

MeshFbx& MeshFbx::operator=(const MeshFbx& other) {
	if (this == &other) {
		return *this;
	}

	mPosition = other.mPosition;
	mNormal = other.mNormal;
	mTexCoord = other.mTexCoord;
	mWeights = other.mWeights;
	mInfluences = other.mInfluences;
	mIndices = other.mIndices;
	UpdateOpenGLBuffers();
	return *this;
}

MeshFbx::~MeshFbx() {
	delete mPosAttrib;
	delete mNormAttrib;
	delete mUvAttrib;
	delete mWeightAttrib;
	delete mInfluenceAttrib;
	delete mIndexBuffer;
}

std::vector<vec3>& MeshFbx::GetPosition() {
	return mPosition;
}
std::vector<vec3>& MeshFbx::GetNormal() {
	return mNormal;
}
std::vector<vec2>& MeshFbx::GetTexCoord() {
	return mTexCoord;
}
std::vector<vec4>& MeshFbx::GetWeights() {
	return mWeights;
}
std::vector<ivec4>& MeshFbx::GetInfluences() {
	return mInfluences;
}
std::vector<unsigned int>& MeshFbx::GetIndices() {
	return mIndices;
}

void MeshFbx::UpdateOpenGLBuffers() {
	if (mPosition.size() > 0) {
		mPosAttrib->Set(mPosition);
	}
	if (mNormal.size() > 0) {
		mNormAttrib->Set(mNormal);
	}
	if (mTexCoord.size() > 0) {
		mUvAttrib->Set(mTexCoord);
	}
	if (mWeights.size() > 0) {
		mWeightAttrib->Set(mWeights);
	}
	if (mInfluences.size() > 0) {
		mInfluenceAttrib->Set(mInfluences);
	}
	if (mIndices.size() > 0) {
		mIndexBuffer->Set(mIndices);
	}
}

void MeshFbx::Bind(int position, int normal, int texCoord,
	int weight, int influcence) {
	if (position >= 0) {
		mPosAttrib->BindTo(position);
	}
	if (normal >= 0) {
		mNormAttrib->BindTo(normal);
	}
	if (texCoord >= 0) {
		mUvAttrib->BindTo(texCoord);
	}
	if (weight >= 0) {
		mWeightAttrib->BindTo(weight);
	}
	if (influcence >= 0) {
		mInfluenceAttrib->BindTo(influcence);
	}
}

void MeshFbx::Draw() {
	if (mIndices.size() > 0) {
		::Draw(*mIndexBuffer, DrawMode::Triangles);
	}
	else {
		::Draw(mPosition.size(), DrawMode::Triangles);
	}
}

void MeshFbx::DrawInstanced(unsigned int numInstances) {
	if (mIndices.size() > 0) {
		::DrawInstanced(*mIndexBuffer, DrawMode::Triangles, numInstances);
	}
	else {
		::DrawInstanced(mPosition.size(), DrawMode::Triangles, numInstances);
	}
}

void MeshFbx::UnBind(int position, int normal, int texCoord, int weight, int influence) {
	if (position >= 0) {
		mPosAttrib->UnBindFrom(position);
	}
	if (normal >= 0) {
		mNormAttrib->UnBindFrom(normal);
	}
	if (texCoord >= 0) {
		mUvAttrib->UnBindFrom(texCoord);
	}
	if (weight >= 0) {
		mWeightAttrib->UnBindFrom(weight);
	}
	if (influence >= 0) {
		mInfluenceAttrib->UnBindFrom(influence);
	}
}

void MeshFbx::CPUSkin(Skeleton& skeleton, Pose& pose) {
	unsigned int numVerts = mPosition.size();
	if (numVerts == 0) { return; }

	mSkinnedPosition.resize(numVerts);
	mSkinnedNormal.resize(numVerts);
	
	pose.GetMatrixPalette(mPosePalette);
	std::vector<mat4> invPosePalette = skeleton.GetInvBindPose();

	for (unsigned int i = 0; i < numVerts; ++i) {
		ivec4& j = mInfluences[i];
		vec4& w = mWeights[i];

		mat4 m0 = (mPosePalette[j.x] * invPosePalette[j.x]) * w.x;
		mat4 m1 = (mPosePalette[j.y] * invPosePalette[j.y]) * w.y;
		mat4 m2 = (mPosePalette[j.z] * invPosePalette[j.z]) * w.z;
		mat4 m3 = (mPosePalette[j.w] * invPosePalette[j.w]) * w.w;

		mat4 skin = m0 + m1 + m2 + m3;

		mSkinnedPosition[i] = transformPoint(skin, mPosition[i]);
		mSkinnedNormal[i] = transformVector(skin, mNormal[i]);
	}

	mPosAttrib->Set(mSkinnedPosition);
	mNormAttrib->Set(mSkinnedNormal);
}

void MeshFbx::CPUSkin(std::vector<mat4>& animatedPose) {
	unsigned int numVerts = mPosition.size();
	if (numVerts == 0) {
		return;
	}

	mSkinnedPosition.resize(numVerts);
	mSkinnedNormal.resize(numVerts);

	for (unsigned int i = 0; i < numVerts; ++i) {
		ivec4& j = mInfluences[i];
		vec4& w = mWeights[i];

		vec3 p0 = transformPoint(animatedPose[j.x], mPosition[i]);
		vec3 p1 = transformPoint(animatedPose[j.y], mPosition[i]);
		vec3 p2 = transformPoint(animatedPose[j.z], mPosition[i]);
		vec3 p3 = transformPoint(animatedPose[j.w], mPosition[i]);

		mSkinnedPosition[i] = p0 * w.x + p1 * w.y + p2 * w.z + p3 * w.w;

		p0 = transformPoint(animatedPose[j.x], mNormal[i]);
		p1 = transformPoint(animatedPose[j.y], mNormal[i]);
		p2 = transformPoint(animatedPose[j.z], mNormal[i]);
		p3 = transformPoint(animatedPose[j.w], mNormal[i]);

		mSkinnedNormal[i] = p0 * w.x + p1 * w.y + p2 * w.z + p3 * w.w;
	}

	mPosAttrib->Set(mSkinnedPosition);
	mNormAttrib->Set(mSkinnedNormal);
}