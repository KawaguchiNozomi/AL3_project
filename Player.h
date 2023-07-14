#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h" 
#include "ViewProjection.h"
#include "ImGuiManager.h"
#include <assert.h>
#include "PlayerBullet.h"
#include "Sprite.h"
#include <list>
class GameScene;


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
	void Initialize(Model*model,uint32_t textureHandle,Vector3 position);
	/// <summary>
	/// 更新
	/// </summary>
	void Update(ViewProjection& viewProjection);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	/// <summary>
	/// 親となるワールドトランスフォームをセット
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransform* parent);

	void DrawUI();

	Vector3 GetWorldPosition();

	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;

	void OnCollision();

	
	const std::list<PlayerBullet*>& GetBullets() { return bullets_; }

private:
	Vector3 GetReticleWorldPos();
	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	Input* input_ = nullptr;
	std::list<PlayerBullet*> bullets_;
	WorldTransform worldTransform3DReticle_;
	Sprite* sprite2DRetecle_ = nullptr;
	ViewProjection viewProjection_;
	/// <summary>
	/// 攻撃
	/// </summary>
	void Attack();
};
