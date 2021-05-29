#pragma once

#include "animation/Pose.h"
#include "animation/Clip.h"
#include "animation/Skeleton.h"

bool IsInHierarchy(Pose& pose, unsigned int parent, unsigned int search);

void Blend(Pose& output, Pose& a, Pose& b, float t, int root);

Pose MakeAdditivePose(Skeleton& skeleton, Clip& clip);

void Add(Pose& output, Pose& inPose, Pose& addPose, Pose& additiveBasePose, int blendroot);