#pragma once

#include "Application.h"
#include "math/vec3.h"
#include "animation/Pose.h"
#include "animation/Clip.h"
#include "animation/Skeleton.h"
#include "render/Mesh.h"
#include "render/Texture.h"
#include "render/Shader.h"
#include "render/DebugDraw.h"
#include <vector>

struct AnimationInstance {
	Pose mAnimatedPose;
	std::vector <mat4> mPosePalette;
	unsigned int mClip;
	float mPlayback;
	Transform mModel;

	inline AnimationInstance() : mClip(0), mPlayback(0.0f) { }
};

class SampleMeshLoading : public Application {
protected:
	Texture* mDiffuseTexture;
	Shader* mStaticShader;
	Shader* mSkinnedShader;
	std::vector<Mesh> mCPUMeshes;
	std::vector<Mesh> mGPUMeshes;
	Skeleton mSkeleton;
	std::vector<Clip> mClips;
	DebugDraw* mRestPoseVisual;

	AnimationInstance mGPUAnimInfo;
	AnimationInstance mCPUAnimInfo;
public:
	void Initialize();
	void Zoom(float zoomDelta);
	void MouseMovement(float x, float y);
	void Update(float deltaTime);
	void Render(float inAspectRatio);
	void ShowDebugUi();
	void Shutdown();

	vec3 m_CameraPosition;
	
	float m_ZoomDelta;
	float m_MouseX;
	float m_MouseY;
};