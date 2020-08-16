#pragma once

#include "animation/cgltf.h"
#include "animation/Pose.h"
#include "animation/Clip.h"
#include "animation/Skeleton.h"

#include "render/Mesh.h"

#include <vector>
#include <string>

cgltf_data* LoadGLTFFile(const char* path);

void FreeGLTFFile(cgltf_data* handle);

void FreeGLTFFile(cgltf_data* handle);

Pose LoadRestPose(cgltf_data* handle);

Pose LoadBindPose(cgltf_data* handle);

Skeleton LoadSkeleton(cgltf_data* handle);

std::vector<std::string> LoadJointNames(cgltf_data* handle);

std::vector<Clip> LoadAnimationClips(cgltf_data* handle);

std::vector<Mesh> LoadMeshes(cgltf_data* handle);