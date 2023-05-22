#include "Enemy.h"
#include <assert.h>

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, const Vector3& position) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemy.png");

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;
	ApproachIntialize();
}

void Enemy::Update() {

	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	switch (phase_) {
	case Phase::Approach:
		ApproachUpdate();
		worldTransform_.translation_.z -=0.2f;
		if (worldTransform_.translation_.z < -4.0f) {
			phase_ = Phase::Leave;
		}
		break;
	case Phase::Leave:
		worldTransform_.translation_.y +=0.2f;
		worldTransform_.translation_.x += 0.2f;
		break;
	default:
		break;
	}
	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::ApproachIntialize() { fireTimer_ = kFireInterval; }

void Enemy::ApproachUpdate() { 
	fireTimer_--;
	if (fireTimer_ == 0) {
		Fire();
		fireTimer_ = kFireInterval;
	}
}

void Enemy::Fire() {
	// 弾を生成し初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_);

	bullets_.push_back(newBullet);
}
