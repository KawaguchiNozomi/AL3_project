#pragma once
#include "Model.h"
#include "WorldTransform.h" 
#include <assert.h>
/// <summary>
/// ©ƒLƒƒƒ‰
/// </summary>
class Player {
public:
	/// <summary>
	/// ‰Šú‰»
	/// </summary>
	void Initialize(Model*model,uint32_t textureHandle);
	/// <summary>
	/// XV
	/// </summary>
	void Update();
	/// <summary>
	/// •`‰æ
	/// </summary>
	void Draw();

private:
	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;

	WorldTransform worldTransform_;
};
