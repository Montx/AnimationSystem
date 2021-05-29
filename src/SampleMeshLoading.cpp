#include "SampleMeshLoading.h"

#pragma warning(disable : 26451)
#define _CRT_SECURE_NO_WARNINGS

#include "animation/GLTFLoader.h"
#include "animation/FbxLoader.h"
#include "render/Uniform.h"
#include "math/Utils.h"
#include "glad.h"

ofbx::IScene* gScene = nullptr;
std::vector<Mesh> loaded_mesh;

void SampleMeshLoading::Initialize() {
	//gScene = LoadFbx("Assets/ybot.fbx");
	//mCPUMeshes = LoadFbxMeshes(*gScene);
	//mSkeleton = LoadFbxSkeleton(*gScene);
	cgltf_data* gltf = LoadGLTFFile("Assets/Woman.gltf");
	mCPUMeshes = LoadMeshes(gltf);
	mSkeleton = LoadSkeleton(gltf);
	mClips = LoadAnimationClips(gltf);
	FreeGLTFFile(gltf);

	mGPUMeshes = mCPUMeshes;
	for (unsigned int i = 0, size = (unsigned int)mGPUMeshes.size(); i < size; ++i) {
		mGPUMeshes[i].UpdateOpenGLBuffers();
	}

	/*mRestPoseVisual = new DebugDraw();
	Pose pose = LoadFbxBindPose(*gScene);
	mRestPoseVisual->FromPose(pose);
	mRestPoseVisual->UpdateOpenGLBuffers();*/

	mStaticShader = new Shader("Shaders/static.vert", "Shaders/lit.frag");
	mSkinnedShader = new Shader("Shaders/skinned.vert", "Shaders/lit.frag");
	mDiffuseTexture = new Texture("Assets/Woman.png");

	mGPUAnimInfo.mAnimatedPose = mSkeleton.GetBindPose();
	mGPUAnimInfo.mPosePalette.resize(mSkeleton.GetBindPose().Size());
	mCPUAnimInfo.mAnimatedPose = mSkeleton.GetBindPose();
	mCPUAnimInfo.mPosePalette.resize(mSkeleton.GetBindPose().Size());

	mGPUAnimInfo.mModel.position = vec3(0, 0, 0);
	mCPUAnimInfo.mModel.position = vec3(0, 0, 0);

	unsigned int numUIClips = (unsigned int)mClips.size();
	for (unsigned int i = 0; i < numUIClips; ++i) {
		if (mClips[i].GetName() == "Walking") {
			mCPUAnimInfo.mClip = i;
		}
		else if (mClips[i].GetName() == "Running") {
			mGPUAnimInfo.mClip = i;
		}
	}

	m_CameraPosition = vec3(0, 5, 7);
}

void SampleMeshLoading::Zoom(float zoomDelta) {
	m_ZoomDelta = zoomDelta;
	m_CameraPosition.z += zoomDelta / 120;
}

void SampleMeshLoading::MouseMovement(float x, float y) {
	m_MouseX = x - (clientWidth * 0.5f);
	m_MouseY = y - (clientHeight * 0.5f);

	m_MouseX = (m_MouseX * 2 * AnimMathUtils::PI) / (clientWidth * 0.5f);
	m_MouseY = (m_MouseY * 1/2 * AnimMathUtils::PI) / (clientHeight * 0.5f);
}

void SampleMeshLoading::Update(float deltaTime) {
	mCPUAnimInfo.mPlayback = mClips[mCPUAnimInfo.mClip].Sample(mCPUAnimInfo.mAnimatedPose, mCPUAnimInfo.mPlayback + deltaTime);
	mGPUAnimInfo.mPlayback = mClips[mGPUAnimInfo.mClip].Sample(mGPUAnimInfo.mAnimatedPose, mGPUAnimInfo.mPlayback + deltaTime);

	for (unsigned int i = 0, size = (unsigned int)mCPUMeshes.size(); i < size; ++i) {
		mCPUMeshes[i].CPUSkin(mSkeleton, mCPUAnimInfo.mAnimatedPose);
		mGPUMeshes[i].CPUSkin(mSkeleton, mGPUAnimInfo.mAnimatedPose);
	}

	mGPUAnimInfo.mAnimatedPose.GetMatrixPalette(mGPUAnimInfo.mPosePalette);
}

void SampleMeshLoading::Render(float inAspectRatio) {
	mat4 projection = perspective(60.0f, inAspectRatio, 0.01f, 1000.0f);
	mat4 view = orbitAround(m_CameraPosition, vec3(0.0f, -m_MouseX, -m_MouseY), vec3(0, 0, 0), vec3(0, 1, 0));
	mat4 model;

	mRestPoseVisual->Draw(DebugDrawMode::Lines, vec3(1, 0, 0), projection * view);

	// CPU Skinned Mesh
	model = transformToMat4(mCPUAnimInfo.mModel);
	mStaticShader->Bind();
	Uniform<mat4>::Set(mStaticShader->GetUniform("model"), model);
	Uniform<mat4>::Set(mStaticShader->GetUniform("view"), view);
	Uniform<mat4>::Set(mStaticShader->GetUniform("projection"), projection);
	Uniform<vec3>::Set(mStaticShader->GetUniform("light"), vec3(1, 1, 1));

	mDiffuseTexture->Set(mStaticShader->GetUniform("tex0"), 0);
	for (unsigned int i = 0, size = (unsigned int)mCPUMeshes.size(); i < size; ++i) {
		mCPUMeshes[i].Bind(mStaticShader->GetAttribute("position"), mStaticShader->GetAttribute("normal"), mStaticShader->GetAttribute("texCoord"), -1, -1);
		mCPUMeshes[i].Draw();
		mCPUMeshes[i].UnBind(mStaticShader->GetAttribute("position"), mStaticShader->GetAttribute("normal"), mStaticShader->GetAttribute("texCoord"), -1, -1);
	}
	mDiffuseTexture->UnSet(0);
	mStaticShader->UnBind();

	// GPU Skinned Mesh
	model = transformToMat4(mGPUAnimInfo.mModel);
	mSkinnedShader->Bind();
	Uniform<mat4>::Set(mSkinnedShader->GetUniform("model"), model);
	Uniform<mat4>::Set(mSkinnedShader->GetUniform("view"), view);
	Uniform<mat4>::Set(mSkinnedShader->GetUniform("projection"), projection);
	Uniform<vec3>::Set(mSkinnedShader->GetUniform("light"), vec3(1, 1, 1));

	Uniform<mat4>::Set(mSkinnedShader->GetUniform("animated"), mCPUAnimInfo.mPosePalette);

	mDiffuseTexture->Set(mSkinnedShader->GetUniform("tex0"), 0);
	for (unsigned int i = 0, size = (unsigned int)mGPUMeshes.size(); i < size; ++i) {
		mGPUMeshes[i].Bind(mSkinnedShader->GetAttribute("position"), mSkinnedShader->GetAttribute("normal"), mSkinnedShader->GetAttribute("texCoord"), mSkinnedShader->GetAttribute("weights"), mSkinnedShader->GetAttribute("joints"));
		mGPUMeshes[i].Draw();
		mGPUMeshes[i].UnBind(mSkinnedShader->GetAttribute("position"), mSkinnedShader->GetAttribute("normal"), mSkinnedShader->GetAttribute("texCoord"), mSkinnedShader->GetAttribute("weights"), mSkinnedShader->GetAttribute("joints"));
	}
	mDiffuseTexture->UnSet(0);
	mSkinnedShader->UnBind();
}

void SampleMeshLoading::ShowDebugUi() {
	//mCPUMeshes = LoadFbxMeshes(*scene);
	//LoadFbxBindPose(*scene);
	/*if (gScene) {
		ShowFbxInspector(gScene);
	}*/
}

void SampleMeshLoading::Shutdown() {
	delete mStaticShader;
	delete mDiffuseTexture;
	delete mSkinnedShader;
	delete mRestPoseVisual;
	mClips.clear();
	mCPUMeshes.clear();
	mGPUMeshes.clear();
}