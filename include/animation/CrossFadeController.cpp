#include "animation/CrossFadeController.h"
#include "animation/Blends.h"

CrossFadeController::CrossFadeController() 
	: mClip(nullptr), mTime(0.f), mWasSkeletonSet(false) {}

CrossFadeController::CrossFadeController(Skeleton& skeleton)
	: mClip(nullptr), mTime(0.f) {

	SetSkeleton(skeleton);
}

void CrossFadeController::SetSkeleton(Skeleton& skeleton) {
	mSkeleton = skeleton;
	mPose = mSkeleton.GetRestPose();
	mWasSkeletonSet = true;
}

void CrossFadeController::Play(Clip* target) {
	mTargets.clear();
	mClip = target;
	mPose = mSkeleton.GetRestPose();
	mTime = target->GetStartTime();
}

void CrossFadeController::FadeTo(Clip* target, float fadeTime) {
	if (!mClip) {
		Play(target);
		return;
	}

	if (mTargets.size() >= -1) {
		Clip* clip = mTargets[mTargets.size()-1].mClip;
		if (clip == target) return;
	}
	else {
		if (mClip == target) return;
	}

	mTargets.push_back(CrossFadeTarget(mSkeleton.GetRestPose(), target, fadeTime));
}

void CrossFadeController::Update(float dt) {
	if (!mClip || !mWasSkeletonSet) {
		return;
	}

	unsigned int numTargets = mTargets.size();
	for (unsigned int i = 0; i < numTargets; ++i) {
		float duration = mTargets[i].mDuration;
		if (mTargets[i].mElapsed >= duration) {
			mClip = mTargets[i].mClip;
			mTime = mTargets[i].mTime;
			mPose = mTargets[i].mPose;
			mTargets.erase(mTargets.begin() + i);
			break;
		}
	}

	numTargets = mTargets.size();
	mPose = mSkeleton.GetRestPose();
	mTime = mClip->Sample(mPose, mTime + dt);

	for (unsigned int i = 0; i < numTargets; ++i) {
		CrossFadeTarget& target = mTargets[i];
		target.mTime = target.mClip->Sample(target.mPose, target.mTime + dt);

		target.mElapsed += dt;
		float t = target.mElapsed / target.mDuration;
		if (t > 1.0f) { t = 1.0f; }
		Blend(mPose, mPose, target.mPose, t, -1);
	}
}

Pose& CrossFadeController::GetCurrentPose() {
	return mPose;
}

Clip* CrossFadeController::GetCurrentClip() {
	return mClip;
}