#pragma once

#include "animation/Interpolation.h"
#include "animation/Frame.h"
#include "math/vec3.h"
#include "math/quat.h"

#include <vector>

template<typename T, int N>
class Track {
public:
	Track();

	void Resize(unsigned int size);
	unsigned int Size();

	Interpolation GetInterpolation();
	void SetInterpolation(Interpolation interp);

	float GetStartTime();
	float GetEndTime();

	T Sample(float time, bool looping);
	Frame<N>& operator[](unsigned int index);

protected:
	T SampleConstant(float time, bool looping);
	T SampleLinear(float time, bool looping);
	T SampleCubic(float time, bool looping);

	T Hermite(float time, const T& p1, const T& s1, const T& p2, const T& s2);

	virtual int FrameIndex(float time, bool looping);

	float AdjustTimeToFitTrack(float t, bool loop);

	T Cast(float* value);

protected:
	std::vector<Frame<N>> mFrames;
	Interpolation mInterpolation;
};

typedef Track<float, 1> ScalarTrack;
typedef Track<vec3, 3> VectorTrack;
typedef Track<quat, 4> QuaternionTrack;

template<typename T, int N>
class FastTrack : public Track<T, N> {
protected:
	std::vector<unsigned int> mSampledFrames;
	int FrameIndex(float time, bool looping) override;
public:
	void UpdateIndexLookupTable();
};

typedef FastTrack<float, 1> ScalarFastTrack;
typedef FastTrack<vec3, 3> VectorFastTrack;
typedef FastTrack<quat, 4> QuaternionFastTrack;

template<typename T, int N>
FastTrack<T, N> OptimizeTrack(Track<T, N>& input);