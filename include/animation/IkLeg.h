#pragma once

#include "animation/CCDSolver.h"
#include "animation/FABRIKSolver.h"
#include "animation/Skeleton.h"
#include "animation/Track.h"
#include "render/DebugDraw.h"

class IKLeg {
protected:
	ScalarTrack mPinTrack;
	FABRIKSolver mSolver;
	Pose mIKPose;

	unsigned int mHipIndex;
	unsigned int mKneeIndex;
	unsigned int mAnkleIndex;
	unsigned int mToeIndex;

	DebugDraw* mLineVisuals;
	DebugDraw* mPointVisuals;

	// The ankle is not flat on the ground. We have to solve taking this into account
	float mAnkleToGroundOffset;

public:
	IKLeg(Skeleton& skeleton, const std::string& hip, const std::string& knee, const std::string& ankle, const std::string& toe);
	IKLeg();
	IKLeg& operator=(const IKLeg&);
	~IKLeg();

	void SolverForLeg(const Transform& model, Pose& pose, const vec3& legColor);

	Pose& GetAdjustedPose();
	ScalarTrack& GetTrack();

	void Draw(const mat4& up, const vec3& legColor);

	unsigned int Hip();
	unsigned int Knee();
	unsigned int Ankle();
	unsigned int Toe();

	void SetAnkleOffset(float offset);
};