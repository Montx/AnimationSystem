#pragma once

#include "Application.h"
#include <vector>
#include "animation/Pose.h"
#include "animation/Clip.h"
#include "animation/Skeleton.h"
#include "render/Texture.h"
#include "render/Mesh.h"
#include <vector>
#include "render/Shader.h"
#include "animation/CrossFadeController.h"
#include "animation/CrossFadeTarget.h"

class Sample : public Application {
protected:
	Shader* mShader;
	Texture* mTexture;
	std::vector<Mesh> mMeshes;
	std::vector<Clip> mClips;
	Skeleton mSkeleton;
	std::vector<mat4> mPosePalette;

	unsigned int mClip;
	unsigned int mAdditiveIndex;

	float mPlaybackTime;
	float mAdditiveTime;
	float mAdditiveDirection;

	Pose mCurrentPose;
	Pose mAddPose;
	Pose mAdditiveBase;
public:
	void Initialize();
	void Update(float inDeltaTime);
	void Render(float inAspectRatio);
	void Shutdown();
};
