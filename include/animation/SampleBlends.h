#pragma once

#include "Application.h"
#include <vector>
#include "Pose.h"
#include "Clip.h"
#include "Skeleton.h"
#include "render/Texture.h"
#include "render/Mesh.h"
#include <vector>
#include "render/Shader.h"

struct AnimationInstance {
	Pose mAnimatedPose;
	std::vector <mat4> mPosePalette;
	unsigned int mClip;
	float mPlayback;
	Transform mModel;

	inline AnimationInstance() : mClip(0), mPlayback(0.0f) { }
};

class Sample : public Application {
protected:
	Texture* mDiffuseTexture;
	Shader* mStaticShader;
	Shader* mSkinnedShader;
	Skeleton mSkeleton;
	std::vector<Clip> mClips;

	std::vector<Mesh> mCPUMeshes;
	std::vector<Mesh> mGPUMeshes;
	AnimationInstance mCPUInstance;
	AnimationInstance mGPUInstance;
public:
	void Initialize();
	void Update(float deltaTime);
	void Render(float inAspectRatio);
	void Shutdown();
};