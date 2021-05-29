#pragma once

#include "animation/ofbx.h"

#include "imgui/imgui.h"

#include <windows.h>
#include <inttypes.h>
#include <cstring>
#include <cstdio>

const ofbx::IElement* g_selected_element = nullptr;
const ofbx::Object* g_selected_object = nullptr;

ofbx::IScene* LoadFbx(const char* path) {
	FILE* fp = nullptr;
	fopen_s(&fp, path, "rb");
	if (!fp) return nullptr;

	fseek(fp, 0, SEEK_END);
	long file_size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	auto* content = new ofbx::u8[file_size];
	fread(content, 1, file_size, fp);

	return ofbx::load((ofbx::u8*)content, file_size, (ofbx::u64)ofbx::LoadFlags::TRIANGULATE);
}

void BuildCurveGui(const ofbx::Object& object) {
	const ofbx::AnimationCurve& curve = static_cast<const ofbx::AnimationCurve&>(object);

	const int c = curve.getKeyCount();
	for (int i = 0; i < c; ++i) {
		const float t = (float)ofbx::fbxTimeToSeconds(curve.getKeyTime()[i]);
		const float v = curve.getKeyValue()[i];
		ImGui::Text("%fs: %f", t, v);
	}
}

void BuildGui(const ofbx::Object& object) {
	const char* label = nullptr;
	switch (object.getType()) {
		case ofbx::Object::Type::GEOMETRY: label = "geometry"; break;
		case ofbx::Object::Type::MESH: label = "mesh"; break;
		case ofbx::Object::Type::MATERIAL: label = "material"; break;
		case ofbx::Object::Type::ROOT: label = "root"; break;
		case ofbx::Object::Type::TEXTURE: label = "texture"; break;
		case ofbx::Object::Type::NULL_NODE: label = "null"; break;
		case ofbx::Object::Type::LIMB_NODE: label = "limb node"; break;
		case ofbx::Object::Type::NODE_ATTRIBUTE: label = "node attribute"; break;
		case ofbx::Object::Type::CLUSTER: label = "cluster"; break;
		case ofbx::Object::Type::SKIN: label = "skin"; break;
		case ofbx::Object::Type::ANIMATION_STACK: label = "animation stack"; break;
		case ofbx::Object::Type::ANIMATION_LAYER: label = "animation layer"; break;
		case ofbx::Object::Type::ANIMATION_CURVE: label = "animation curve"; break;
		case ofbx::Object::Type::ANIMATION_CURVE_NODE: label = "animation curve node"; break;
		case ofbx::Object::Type::POSE: label = "pose"; break;
		case ofbx::Object::Type::SHAPE: label = "shape"; break;
		case ofbx::Object::Type::BLEND_SHAPE: label = "blend_shape"; break;
		case ofbx::Object::Type::BLEND_SHAPE_CHANNEL: label = "blend_shape_channel"; break;
		default: assert(false); break;
	}

	ImGuiTreeNodeFlags flags = g_selected_object == &object ? ImGuiTreeNodeFlags_Selected : 0;
	
	char tmp[128];
	sprintf_s(tmp, "%" PRId64 " %s (%s)", object.id, object.name, label);

	if (ImGui::TreeNodeEx(tmp, flags)) {
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) g_selected_object = &object;
		
		int i = 0;
		while (ofbx::Object* child = object.resolveObjectLink(i)) {
			BuildGui(*child);
			++i;
		}

		if (object.getType() == ofbx::Object::Type::ANIMATION_CURVE) {
			BuildCurveGui(object);
		}

		ImGui::TreePop();
	}
	else {
		if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) g_selected_object = &object;
	}
}

template <int N>
void catProperty(char(&out)[N], const ofbx::IElementProperty& prop)
{
	char tmp[128];
	switch (prop.getType())
	{
		case ofbx::IElementProperty::DOUBLE: sprintf_s(tmp, "%f", prop.getValue().toDouble()); break;
		case ofbx::IElementProperty::LONG: sprintf_s(tmp, "%" PRId64, prop.getValue().toU64()); break;
		case ofbx::IElementProperty::INTEGER: sprintf_s(tmp, "%d", prop.getValue().toInt()); break;
		case ofbx::IElementProperty::STRING: prop.getValue().toString(tmp); break;
		default: sprintf_s(tmp, "Type: %c", (char)prop.getType()); break;
	}
	strcat_s(out, tmp);
}

void BuildElementsGui(const ofbx::IElement& parent) {
	for (const ofbx::IElement* element = parent.getFirstChild(); element; element = element->getSibling()) {
		auto id = element->getID();
		char label[128];
		id.toString(label);
		strcat_s(label, " (");
		ofbx::IElementProperty* prop = element->getFirstProperty();
		bool first = true;

		while (prop) {
			if (!first) {
				strcat_s(label, ", ");
			}
			first = false;
			catProperty(label, *prop);
			prop = prop->getNext();
		}
		strcat_s(label, ")");

		ImGui::PushID((const void*)id.begin);
		ImGuiTreeNodeFlags flags = g_selected_element == element ? ImGuiTreeNodeFlags_Selected : 0;

		if (!element->getFirstChild()) flags |= ImGuiTreeNodeFlags_Leaf;

		if (ImGui::TreeNodeEx(label, flags)) {
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) g_selected_element = element;
			if (element->getFirstChild()) BuildElementsGui(*element);
			ImGui::TreePop();
		}
		else {
			if (ImGui::IsItemHovered() && ImGui::IsMouseClicked(0)) g_selected_element = element;
		}
		ImGui::PopID();
	}
}

std::vector<std::string> GetJointNames(const ofbx::IScene& scene) {
	const ofbx::Mesh* mesh = scene.getMesh(0);
	const ofbx::Skin* skin = mesh->getGeometry()->getSkin();
	int total_joints = skin->getClusterCount();

	std::vector<std::string> joint_names;
	joint_names.reserve(total_joints);

	for (int i = 0; i < total_joints; i++) {
		const ofbx::Cluster& joint = *skin->getCluster(i);

		joint_names.emplace_back(joint.name);
	}

	return joint_names;
}

void ShowMeshInspector(const ofbx::IScene* scene) {
	int mesh_count = scene->getMeshCount();

	for (int i = 0; i < mesh_count; ++i) {
		const ofbx::Mesh* mesh = scene->getMesh(i);

		if (!ImGui::TreeNode(mesh->name)) continue;

		const ofbx::Geometry* geom = mesh->getGeometry();
		const ofbx::Vec3* vertices = geom->getVertices();

		const ofbx::Pose* pose = mesh->getPose();

		char tmp[128];
		sprintf_s(tmp, "Skeleton");

		if (ImGui::TreeNode(tmp)) {
			
			std::vector<std::string> joint_names = GetJointNames(*scene);

			for (int i = 0; i < joint_names.size(); i++) {
				ImGui::BulletText(joint_names[i].c_str());
			}
			
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

template <int N>
void toString(ofbx::DataView view, char(&out)[N])
{
	int len = int(view.end - view.begin);
	if (len > sizeof(out) - 1) len = sizeof(out) - 1;
	strncpy_s(out, (const char*)view.begin, len);
	out[len] = 0;
}

template <typename T>
void showArray(const char* label, const char* format, ofbx::IElementProperty& prop)
{
	if (!ImGui::CollapsingHeader(label)) return;

	int count = prop.getCount();
	ImGui::Text("Count: %d", count);
	std::vector<T> tmp;
	tmp.resize(count);
	prop.getValues(&tmp[0], int(sizeof(tmp[0]) * tmp.size()));
	for (T v : tmp)
	{
		ImGui::Text(format, v);
	}
}

void BuildElementPropertyGui(ofbx::IElementProperty& prop) {
	ImGui::PushID((void*)&prop);
	char tmp[256];
	switch (prop.getType()) {
		case ofbx::IElementProperty::LONG: ImGui::Text("Long: %" PRId64, prop.getValue().toU64()); break;
		case ofbx::IElementProperty::FLOAT: ImGui::Text("Float: %f", prop.getValue().toFloat()); break;
		case ofbx::IElementProperty::DOUBLE: ImGui::Text("Double: %f", prop.getValue().toDouble()); break;
		case ofbx::IElementProperty::INTEGER: ImGui::Text("Integer: %d", prop.getValue().toInt()); break;
		case ofbx::IElementProperty::ARRAY_FLOAT: showArray<float>("float array", "%f", prop); break;
		case ofbx::IElementProperty::ARRAY_DOUBLE: showArray<double>("double array", "%f", prop); break;
		case ofbx::IElementProperty::ARRAY_INT: showArray<int>("int array", "%d", prop); break;
		case ofbx::IElementProperty::ARRAY_LONG: showArray<ofbx::u64>("long array", "%" PRId64, prop); break;
		case ofbx::IElementProperty::STRING:
			toString(prop.getValue(), tmp);
			ImGui::Text("String: %s", tmp);
			break;
		default:
			ImGui::Text("Other: %c", (char)prop.getType());
			break;
	}

	ImGui::PopID();
	if (prop.getNext()) BuildElementPropertyGui(*prop.getNext());
}

void ShowFbxInspector(const ofbx::IScene* scene) {
	ImGui::Begin("Fbx Inspector");

	if (ImGui::CollapsingHeader("Elements")) {
		const ofbx::IElement* root = scene->getRootElement();
		if (root && root->getFirstChild()) BuildElementsGui(*root);
	}

	if (ImGui::Begin("Properties") && g_selected_element) {
		ofbx::IElementProperty* prop = g_selected_element->getFirstProperty();
		if (prop) BuildElementPropertyGui(*prop);
	}
	ImGui::End();

	if (ImGui::CollapsingHeader("Objects")) {
		const ofbx::Object* root = scene->getRoot();
		if (root) BuildGui(*root);

		int count = scene->getAnimationStackCount();
		for (int i = 0; i < count; ++i) {
			const ofbx::Object* stack = scene->getAnimationStack(i);
			BuildGui(*stack);
		}
	}

	if (ImGui::CollapsingHeader("Meshes")) {
		ShowMeshInspector(scene);
	}

	ImGui::End();
}

Transform OfbxMatrixToTransform(ofbx::Matrix matrix) {
	mat4 mat(&matrix.m[0]);
	return mat4ToTransform(mat);
}

Pose LoadFbxBindPose(const ofbx::IScene& scene) {
	const ofbx::Mesh* mesh = scene.getMesh(0);
	const ofbx::Skin* skin = mesh->getGeometry()->getSkin();
	Pose result(skin->getClusterCount());

	for (int i = 0; i < result.Size(); i++) {
		const ofbx::Cluster& joint = *skin->getCluster(i);

		result.SetLocalTransform(i, OfbxMatrixToTransform(joint.getLink()->getLocalTransform()));

		const ofbx::Object* parent = joint.getLink()->getParent();

		if (i == 0) {
			result.SetParent(i, -1);
			continue;
		}

		for (int ip = 0; ip < result.Size(); ip++) {
			const ofbx::Object* potential_parent = skin->getCluster(ip)->getLink();

			if (parent == potential_parent) {
				result.SetParent(i, ip);
				break;
			}
		}
	}

	return result;
}

Skeleton LoadFbxSkeleton(const ofbx::IScene& scene) {
	return Skeleton(
		LoadFbxBindPose(scene),
		GetJointNames(scene)
	);
}

//std::vector<std::string> LoadJointNames(cgltf_data* handle);
//
//std::vector<Clip> LoadAnimationClips(cgltf_data* handle);

std::vector<Mesh> LoadFbxMeshes(const ofbx::IScene& scene) {
	std::vector<Mesh> result;
	int mesh_count = scene.getMeshCount();

	for (int i = 0; i < mesh_count; ++i) {
		result.emplace_back();
		Mesh& out_mesh = result[result.size() - 1];

		const ofbx::Mesh* mesh = scene.getMesh(i);
		const ofbx::Geometry* geom = mesh->getGeometry();

		std::vector<vec3>& out_vertices = out_mesh.GetPosition();
		const ofbx::Vec3* vertices = geom->getVertices();

		int vertex_count = geom->getVertexCount();
		for (int i = 0; i < vertex_count; ++i) {
			ofbx::Vec3 vertex = vertices[i];
			out_vertices.emplace_back(vertex.x / 100, vertex.y / 100, vertex.z /100);
		}

		std::vector<vec3>& out_normals = out_mesh.GetNormal();
		const ofbx::Vec3* normals = geom->getNormals();

		int count = geom->getIndexCount();

		for (int i = 0; i < count; ++i) {
			ofbx::Vec3 normal = normals[i];
			out_normals.emplace_back(normal.x / 100, normal.y / 100, normal.z / 100);
		}

		std::vector<vec2>& out_texCoords = out_mesh.GetTexCoord();
		const ofbx::Vec2* uvs = geom->getUVs();

		for (int i = 0; i < count; ++i) {
			ofbx::Vec2 uv = uvs[i];
			out_texCoords.emplace_back(uv.x, uv.y);
		}

		std::vector<unsigned int>& out_indices = out_mesh.GetIndices();
		const int* indices = geom->getFaceIndices();

		for (int i = 0; i < count; ++i) {
			out_indices.emplace_back(indices[i]);
		}

		std::vector<ivec4>& out_joints_index = out_mesh.GetInfluences();
		out_joints_index.insert(out_joints_index.end(), count, {-1, -1, -1, -1});

		std::vector<vec4>& out_weights = out_mesh.GetWeights();
		out_weights.insert(out_weights.end(), count, {});

		const ofbx::Skin* skin = geom->getSkin();
		int cluster_count = skin->getClusterCount();

		for (int cid = 0; cid < cluster_count; ++cid) {
			const ofbx::Cluster* cluster = skin->getCluster(cid);

			const int ic = cluster->getIndicesCount();
			for (int i = 0; i < ic; ++i) {
				int current_index = cluster->getIndices()[i];

				ivec4& current_linked_joints = out_joints_index[current_index];

				if (current_linked_joints.x == -1) {
					current_linked_joints.x = cid;
					out_weights[current_index].x = cluster->getWeights()[i];
				}else if(current_linked_joints.y == -1) {
					current_linked_joints.y = cid;
					out_weights[current_index].y = cluster->getWeights()[i];
				} else if (current_linked_joints.z == -1) {
					current_linked_joints.z = cid;
					out_weights[current_index].z = cluster->getWeights()[i];
				} else if (current_linked_joints.w == -1) {
					current_linked_joints.w = cid;
					out_weights[current_index].w = cluster->getWeights()[i];
				}
			}
		}

		
	}

	return result;
}