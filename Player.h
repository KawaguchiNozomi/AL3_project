#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h" 
#include "ViewProjection.h"
#include <assert.h>
/// <summary>
/// 
/// </summary>
class Player {
public:
	/// <summary>
	/// 
	/// </summary>
	void Initialize(Model*model,uint32_t textureHandle);
	/// <summary>
	/// 
	/// </summary>
	void Update();
	/// <summary>
	/// 
	/// </summary>
	void Draw(ViewProjection viewProjection);

private:
	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	Input* input_ = nullptr;
};
