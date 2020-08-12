#pragma once

#include "animation/TransformTrack.h"
#include "animation/Pose.h"

#include <vector>
#include <string>

class Clip {
public:
	Clip();

	unsigned int GetIdAtIndex(unsigned int index);
	void SetIdAtIndex(unsigned int idx, unsigned int id);

	unsigned int Size();

	float Sample(Pose& outPose, float inTime);
	
	TransformTrack& operator[](unsigned int index);

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
	std::vector<TransformTrack> mTracks;
	std::string mName;
	float mStartTime;
	float mEndTime;
	bool mLooping;
};