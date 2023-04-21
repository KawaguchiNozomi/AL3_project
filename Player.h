#pragma once
#include "Model.h"
#include "WorldTransform.h" 
#include <assert.h>
/// <summary>
/// ���L����
/// </summary>
class Player {
public:
	/// <summary>
	/// ������
	/// </summary>
	void Initialize(Model*model,uint32_t textureHandle);
	/// <summary>
	/// �X�V
	/// </summary>
	void Update();
	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

private:
	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;

	WorldTransform worldTransform_;
};
