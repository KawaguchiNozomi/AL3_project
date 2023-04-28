#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h" 
#include "ViewProjection.h"
#include "ImGuiManager.h"
#include <assert.h>
/// <summary>
/// 
/// </summary>
class Player {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model*model,uint32_t textureHandle);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

private:
	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	Input* input_ = nullptr;
};
