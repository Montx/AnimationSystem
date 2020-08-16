#include "render/Mesh.h"

#include "render/Draw.h"

Mesh::Mesh()
	: mPosAttrib(new Attribute<vec3>())
	, mNormAttrib(new Attribute<vec3>())
	, mUvAttrib(new Attribute<vec2>())
	, mWeightAttrib(new Attribute<vec4>())
	, mInfluenceAttrib(new Attribute<ivec4>())
	, mIndexBuffer(new IndexBuffer()) {}

Mesh::Mesh(const Mesh& other)
	: mPosAttrib(new Attribute<vec3>())
	, mNormAttrib(new Attribute<vec3>())
	, mUvAttrib(new Attribute<vec2>())
	, mWeightAttrib(new Attribute<vec4>())
	, mInfluenceAttrib(new Attribute<ivec4>())
	, mIndexBuffer(new IndexBuffer()) {

	*this = other;
}

Mesh& Mesh::operator=(const Mesh& other) {
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

Mesh::~Mesh() {
	delete mPosAttrib;
	delete mNormAttrib;
	delete mUvAttrib;
	delete mWeightAttrib;
	delete mInfluenceAttrib;
	delete mIndexBuffer;
}

std::vector<vec3>& Mesh::GetPosition() {
	return mPosition;
}
std::vector<vec3>& Mesh::GetNormal() {
	return mNormal;
}
std::vector<vec2>& Mesh::GetTexCoord() {
	return mTexCoord;
}
std::vector<vec4>& Mesh::GetWeights() {
	return mWeights;
}
std::vector<ivec4>& Mesh::GetInfluences() {
	return mInfluences;
}
std::vector<unsigned int>& Mesh::GetIndices() {
	return mIndices;
}

void Mesh::UpdateOpenGLBuffers() {
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

void Mesh::Bind(int position, int normal, int texCoord,
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

void Mesh::Draw() {
	if (mIndices.size() > 0) {
		::Draw(*mIndexBuffer, DrawMode::Triangles);
	}
	else {
		::Draw(mPosition.size(), DrawMode::Triangles);
	}
}

void Mesh::DrawInstanced(unsigned int numInstances) {
	if (mIndices.size() > 0) {
		::DrawInstance(*mIndexBuffer, DrawMode::Triangles, numInstances);
	}
	else {
		::DrawInstance(mPosition.size(), DrawMode::Triangles, numInstances);
	}
}

void Mesh::UnBind(int position, int normal, int texCoord, int weight, int influence) {
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

void Mesh::CPUSkin(Skeleton& skeleton, Pose& pose) {
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