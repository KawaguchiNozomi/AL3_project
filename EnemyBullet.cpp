#include "EnemyBullet.h"
#include <assert.h>

void EnemyBullet::Initialize(Model* model, const Vector3& position,const Vector3&velocity) {
	// NULLポインタチェック
	assert(model);
	model_ = model;
	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("enemyBullet.png");

	worldTransform_.Initialize();
	// 引数で受け取った初期座標をセット
	worldTransform_.translation_ = position;

	velocity_ = velocity;

	SetCollisionAttribute(CollisionConfig::kCollisionAttributeEnemy_);
	SetCollisionMask(CollisionConfig::kCollisionAttributePlayer_);
}

void EnemyBullet::Update() {
	// 座標を移動させる
	worldTransform_.translation_.x += velocity_.x;
	worldTransform_.translation_.y += velocity_.y;
	worldTransform_.translation_.z += velocity_.z;
	// ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	if (--dethTimer <= 0) {
		isDead_ = true;
	}
}

void EnemyBullet::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}

Vector3 EnemyBullet::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void EnemyBullet::OnCollision() { 
	isDead_ = true;
}
