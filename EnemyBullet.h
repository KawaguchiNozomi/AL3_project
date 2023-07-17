#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Collider.h"

class EnemyBullet : public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="position">初期座標</param>
	void Initialize(Model* model, const Vector3& position,const Vector3&velocity);
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	bool IsDead() const { return isDead_; }

	
	Vector3 GetWorldPosition() override;

	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;

	void OnCollision() override;


private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	Vector3 velocity_ = {0,0,-1.0f};
	static const int32_t kLifeTime = 60 * 3;
	int32_t dethTimer = kLifeTime;
	bool isDead_ = false;
};