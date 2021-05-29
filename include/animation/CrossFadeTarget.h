#pragma once

#include "animation/Pose.h"
#include "animation/Clip.h"

struct CrossFadeTarget {
	Pose mPose; // Target animation sampled pose
	Clip* mClip; // Target animation clip
	float mTime;  // Target animation elapsed time
	float mDuration; // Fade duration
	float mElapsed; // Current Fade Time

	inline CrossFadeTarget() 
		: mClip(0), mTime(0.0f), mDuration(0.0f), mElapsed(0.0f) {}

	inline CrossFadeTarget(Pose& pose, Clip* target, float duration) 
		: mPose(pose), mClip(target), mTime(target->GetStartTime()), mDuration(duration), mElapsed(0.0f) {}
};