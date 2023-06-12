#pragma once
#include "WorldTransform.h"
#include "ViewProjection.h"
class RailCamera {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Vector3& trans, Vector3& rotate);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	const ViewProjection& GetViewProjection() { return viewprojection_; }
	const WorldTransform& GetWorldTransform() { return worldTransform_; }

private:

	WorldTransform worldTransform_;
	ViewProjection viewprojection_;
};
