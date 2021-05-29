#include "animation/Clip.h"

template <typename TRACK>
TClip<TRACK>::TClip() 
	: mName("No name given")
	, mStartTime(0.0f)
	, mEndTime(0.0f)
	, mLooping(true) {}

template <typename TRACK>
float TClip<TRACK>::Sample(Pose& outPose, float time) {
	if (GetDuration() == 0.0f) {
		return 0.0f;
	}

	time = AdjustTimeToFitRange(time);

	unsigned int size = mTracks.size();

	for (unsigned int i = 0; i < size; ++i) {
		unsigned int j = mTracks[i].GetId(); // Joint Id

		Transform local = outPose.GetLocalTransform(j);
		Transform animated = mTracks[i].Sample(local, time, mLooping);

		outPose.SetLocalTransform(j, animated);
	}

	return time;
}

template <typename TRACK>
float TClip<TRACK>::AdjustTimeToFitRange(float inTime) {
	if (mLooping) {
		float duration = mEndTime - mStartTime;
		if (duration <= 0) {
			return 0.0f;
		}

		inTime = fmodf(inTime - mStartTime, mEndTime - mStartTime);

		if (inTime < 0.0f) {
			inTime += mEndTime - mStartTime;
		}

		inTime = inTime + mStartTime;
	}
	else {
		if (inTime < mStartTime) {
			inTime = mStartTime;
		}

		if (inTime > mEndTime) {
			inTime = mEndTime;
		}
	}

	return inTime;
}

template <typename TRACK>
void TClip<TRACK>::RecalculateDuration() {
	mStartTime = 0.0f;
	mEndTime = 0.0f;
	bool startSet = false;
	bool endSet = false;
	unsigned int tracksSize = mTracks.size();

	for (unsigned int i = 0; i < tracksSize; ++i) {
		if (mTracks[i].IsValid()) {
			float startTime = mTracks[i].GetStartTime();
			float endTime = mTracks[i].GetEndTime();

			if (startTime < mStartTime || !startSet) {
				mStartTime = startTime;
				startSet = true;
			}

			if (endTime > mEndTime || !endSet) {
				mEndTime = endTime;
				endSet = true;
			}
		}
	}
}

template <typename TRACK>
TRACK& TClip<TRACK>::operator[](unsigned int joint) {
	for (int i = 0, s = mTracks.size(); i < s; ++i) {
		if (mTracks[i].GetId() == joint) {
			return mTracks[i];
		}
	}

	mTracks.push_back(TRACK());
	mTracks[mTracks.size() - 1].SetId(joint);
	return mTracks[mTracks.size() - 1];
}

template <typename TRACK>
std::string& TClip<TRACK>::GetName() {
	return mName;
}

template <typename TRACK>
unsigned int TClip<TRACK>::GetIdAtIndex(unsigned int index) {
	return mTracks[index].GetId();
}

template <typename TRACK>
unsigned int TClip<TRACK>::Size() {
	return (unsigned int)mTracks.size();
}

template <typename TRACK>
float TClip<TRACK>::GetDuration() {
	return mEndTime - mStartTime;
}

template <typename TRACK>
float TClip<TRACK>::GetStartTime() {
	return mStartTime;
}

template <typename TRACK>
float TClip<TRACK>::GetEndTime() {
	return mEndTime;
}

template <typename TRACK>
bool TClip<TRACK>::GetLooping() {
	return mLooping;
}

template <typename TRACK>
void TClip<TRACK>::SetName(const std::string& inNewName) {
	mName = inNewName;
}

template <typename TRACK>
void TClip<TRACK>::SetIdAtIndex(unsigned int index, unsigned int id) {
	return mTracks[index].SetId(id);
}

template <typename TRACK>
void TClip<TRACK>::SetLooping(bool inLooping) {
	mLooping = inLooping;
}

template TClip<TransformTrack>;
template TClip<FastTransformTrack>;

FastClip OptimizeClip(Clip& input) {
	FastClip result;
	result.SetName(input.GetName());
	result.SetLooping(input.GetLooping());
	unsigned int size = input.Size();
	for (unsigned int i = 0; i < size; ++i) {
		unsigned int joint = input.GetIdAtIndex(i);
		result[joint] = OptimizeTransformTrack(input[joint]);
	}

	result.RecalculateDuration();
	return result;
}