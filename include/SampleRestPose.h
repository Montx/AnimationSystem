#pragma once

#include "Application.h"

#include "render/DebugDraw.h"
#include "animation/Pose.h"
#include "animation/Clip.h"

#include <vector>

class SampleRestPose : public Application {
public:
	void Initialize();
	void Update(float deltaTime);
	void Render(float inAspectRatio);
	void Shutdown();

protected:
	Pose mRestPose;
	Pose mCurrentPose;
	std::vector<Clip> mClips;
	unsigned int mCurrentClip;
	float mPlaybackTime;
	DebugDraw* mRestPoseVisual;
	DebugDraw* mCurrentPoseVisual;
};