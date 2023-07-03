#include "RailCamera.h"
#include <ImGuiManager.h>

void RailCamera::Initialize(Vector3& trans,Vector3& rotate) { 
	worldTransform_.Initialize();
	worldTransform_.translation_ = trans;
	worldTransform_.rotation_ = rotate;
	viewprojection_.Initialize();
}

void RailCamera::Update() { 

	Vector3 move = {0.0f, 0.0f, -0.05f};
	Vector3 rot = {0.0f, -0.0001f, 0.0f};

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	//worldTransform_.rotation_.x += rot.x;
	//worldTransform_.rotation_.y += rot.y;
	//worldTransform_.rotation_.z += rot.z;

	worldTransform_.UpdateMatrix();
	viewprojection_.matView = Inverse(worldTransform_.matWorld_);
	viewprojection_.TransferMatrix();

#ifdef _DEBUG
	float trans[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	float rotation[3] = {
	    worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z};
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("translation", trans, -20.0f, 20.0f);
	ImGui::SliderFloat3("rotation", rotation, -5.0f, 5.0f);
	ImGui::End();

	worldTransform_.translation_.x = trans[0];
	worldTransform_.translation_.y = trans[1];
	worldTransform_.translation_.z = trans[2];

	worldTransform_.rotation_.x = rotation[0];
	worldTransform_.rotation_.y = rotation[1];
	worldTransform_.rotation_.z = rotation[2];

#endif // _DEBUG
}
