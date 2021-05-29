#pragma once
#pragma once

#include "animation/CrossFadeTarget.h"
#include "animation/Skeleton.h"

#include <vector>

class CrossFadeController {
protected:
	std::vector<CrossFadeTarget> mTargets;
	Clip* mClip;
	float mTime;
	Pose mPose;
	Skeleton mSkeleton;
	bool mWasSkeletonSet;

public:
	CrossFadeController();
	CrossFadeController(Skeleton& skeleton);
	void SetSkeleton(Skeleton& skeleton);
	void Play(Clip* target); // Used to set the main animation playing
	void FadeTo(Clip* target, float fadeTime);  // Used to fade in new animations
	void Update(float dt);
	Pose& GetCurrentPose();
	Clip* GetCurrentClip();
};