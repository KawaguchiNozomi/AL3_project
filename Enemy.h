﻿#pragma once
#include "Model.h"
#include "Vector3.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include <list>
//前方宣言
class Player;

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
	void SetPlayer(Player* player) { player_ = player; }
	Vector3 GetWorldPosition();

	const float GetRadius() { return radius_; }
	const float radius_ = 1.0f;

	void OnCollision();

	const std::list<EnemyBullet*>& GetBullets() { return bullets_; }

	//発車時間
	static const int kFireInterval = 30;

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
	Player* player_ = nullptr;
	void Fire();
};
