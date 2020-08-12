#include "SampleTextureRender.h"
#include "math/mat4.h"
#include "math/quat.h"
#include "render/Uniform.h"
#include "render/Draw.h"

void SampleTextureRender::Initialize() {
	mRotation = 0.0f;
	mTranslation = vec3(0, 0, 0);
	mShader = new Shader("shaders/static.vert", "shaders/lit.frag");
	mDisplayTexture = new Texture("assets/uv.png");

	mVertexPositions = new Attribute<vec3>();
	mVertexNormals = new Attribute<vec3>();
	mVertexTexCoords = new Attribute<vec2>();
	mIndexBuffer = new IndexBuffer();

	std::vector<vec3> positions;
	positions.push_back(vec3(-1, -1, 0));
	positions.push_back(vec3(-1, 1, 0));
	positions.push_back(vec3(1, -1, 0));
	positions.push_back(vec3(1, 1, 0));
	mVertexPositions->Set(positions);

	std::vector<vec3> normals;
	normals.resize(4, vec3(0, 0, 1));
	mVertexNormals->Set(normals);

	std::vector<vec2> uvs;
	uvs.push_back(vec2(0, 0));
	uvs.push_back(vec2(0, 1));
	uvs.push_back(vec2(1, 0));
	uvs.push_back(vec2(1, 1));
	mVertexTexCoords->Set(uvs);

	std::vector<unsigned int> indices;
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);
	mIndexBuffer->Set(indices);
}

void SampleTextureRender::Update(float inDeltaTime) {
	//mRotation += inDeltaTime * 45.0f;
	//while (mRotation > 360.0f) {
	//	mRotation -= 360.0f;
	//}

	//if (mTranslation.x < 10) {
	//	mTranslation.x += 1 * inDeltaTime;
	//}
}

void SampleTextureRender::Render(float inAspectRatio) {
	mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
	mat4 view = lookAt(vec3(0, 0, -5), mTranslation, vec3(0, 1, 0));
	mat4 model = quatToMat4(angleAxis(mRotation * DEG2RAD, vec3(0, 0, 1)));

	model.c3r0 = mTranslation.x;
	model.c3r1 = mTranslation.y;
	model.c3r2 = mTranslation.z;

	mShader->Bind();

	mVertexPositions->BindTo(mShader->GetAttribute("position"));
	mVertexNormals->BindTo(mShader->GetAttribute("normal"));
	mVertexTexCoords->BindTo(mShader->GetAttribute("texCoord"));

	Uniform<mat4>::Set(mShader->GetUniform("model"), model);
	Uniform<mat4>::Set(mShader->GetUniform("view"), view);
	Uniform<mat4>::Set(mShader->GetUniform("projection"), projection);

	Uniform<vec3>::Set(mShader->GetUniform("light"), vec3(0, 0, 1));

	mDisplayTexture->Set(mShader->GetUniform("tex0"), 0);

	Draw(*mIndexBuffer, DrawMode::Triangles);

	mDisplayTexture->UnSet(0);

	mVertexPositions->UnBindFrom(mShader->GetAttribute("position"));
	mVertexNormals->UnBindFrom(mShader->GetAttribute("normal"));
	mVertexTexCoords->UnBindFrom(mShader->GetAttribute("texCoord"));

	mShader->UnBind();
}

void SampleTextureRender::Shutdown() {
	delete mShader;
	delete mDisplayTexture;
	delete mVertexPositions;
	delete mVertexNormals;
	delete mVertexTexCoords;
	delete mIndexBuffer;
}