#pragma once

#include "animation/Track.h"
#include "math/Transform.h"

template <typename VTRACK, typename QTRACK>
class TTransformTrack {
public:
	TTransformTrack();

	unsigned int GetId();
	void SetId(unsigned int id);

	VTRACK& GetPositionTrack();
	QTRACK& GetRotationTrack();
	VTRACK& GetScaleTrack();

	float GetStartTime();
	float GetEndTime();

	bool IsValid();

	Transform Sample(const Transform& ref, float time, bool looping);

protected:
	unsigned int mId;
	VTRACK mPosition;
	QTRACK mRotation;
	VTRACK mScale;
};

typedef TTransformTrack<VectorTrack, QuaternionTrack> TransformTrack;
typedef TTransformTrack<VectorFastTrack, QuaternionFastTrack> FastTransformTrack;

FastTransformTrack OptimizeTransformTrack(TransformTrack& input);