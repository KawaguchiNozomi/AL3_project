#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h" 
#include "ViewProjection.h"
#include "ImGuiManager.h"
#include <assert.h>
#include "PlayerBullet.h"
#include <list>
/// <summary>
/// 
/// </summary>
class Player {
public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();
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

	Vector3 GetWorldPosition();

	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;

	void OnCollision();

	
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

private:
	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	Input* input_ = nullptr;
	std::list<PlayerBullet*> bullets_;
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();

};
