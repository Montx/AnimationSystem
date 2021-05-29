#pragma once

#include "animation/TransformTrack.h"
#include "animation/Pose.h"

#include <vector>
#include <string>

template <typename TRACK>
class TClip {
public:
	TClip();

	unsigned int GetIdAtIndex(unsigned int index);
	void SetIdAtIndex(unsigned int idx, unsigned int id);

	unsigned int Size();

	float Sample(Pose& outPose, float inTime);
	
	TRACK& operator[](unsigned int index);

	void RecalculateDuration();

	std::string& GetName();
	void SetName(const std::string& inNewName);

	float GetDuration();
	float GetStartTime();
	float GetEndTime();

	bool GetLooping();
	void SetLooping(bool inLooping);

protected:
	float AdjustTimeToFitRange(float inTime);

protected:
	std::vector<TRACK> mTracks;
	std::string mName;
	float mStartTime;
	float mEndTime;
	bool mLooping;
};

typedef TClip<TransformTrack> Clip;
typedef TClip<FastTransformTrack> FastClip;

FastClip OptimizeClip(Clip& input);