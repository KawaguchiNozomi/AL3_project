#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include <list>
/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	~Enemy();
	/// <summary>
	/// 敵の初期化
	/// </summary>
	void Initialize(Model* model, const Vector3& position);
	/// <summary>
	/// 敵の更新
	/// </summary>
	void Update();
	/// <summary>
	/// 敵の描画
	/// </summary>
	void Draw(const ViewProjection& viewProjection);

	void ApproachIntialize();
	void ApproachUpdate();
	//発車時間
	static const int kFireInterval = 60;

private:
	enum class Phase {
		Approach, // 近接する
		Leave,    // 離脱する
	};
	Phase phase_ = Phase::Approach;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0;
	std::list<EnemyBullet*> bullets_;
	//発射タイマー
	int32_t fireTimer_ = 0;
	void Fire();
};
