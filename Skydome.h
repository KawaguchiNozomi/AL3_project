#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "ViewProjection.h"
class Skydome {

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	//uint32_t textureHandle_;
};
