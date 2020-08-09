#pragma once

#include "animation/Track.h"
#include "math/Transform.h"

class TransformTrack {
public:
	TransformTrack();

	unsigned int GetId();
	void SetId(unsigned int id);

	VectorTrack& GetPositionTrack();
	QuaternionTrack& GetRotationTack();
	VectorTrack& GetScaleTrack();

	float GetStartTime();
	float GetEndTime();

	bool IsValid();

	Transform Sample(const Transform& ref, float time, bool looping);

protected:
	unsigned int mId;
	VectorTrack mPosition;
	QuaternionTrack mRotation;
	VectorTrack mScale;
};