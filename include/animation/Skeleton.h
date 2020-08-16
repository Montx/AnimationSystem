#pragma once

#include "animation/Pose.h"
#include "math/mat4.h"

#include <vector>
#include <string>

class Skeleton {
public:
	Skeleton();
	Skeleton(const Pose& rest, const Pose& bind, const std::vector<std::string>& names);

	void Set(const Pose& rest, const Pose& bind, const std::vector<std::string>& names);

	Pose& GetBindPose();
	Pose& GetRestPose();
	std::vector<mat4>& GetInvBindPose();
	std::vector<std::string>& GetJointNames();
	std::string& GetJointName(unsigned int index);

protected:
	void UpdateInverseBindPose();

protected:
	Pose mRestPose;
	Pose mBindPose;
	std::vector<mat4> mInvBindPose;
	std::vector<std::string> mJointNames;
};