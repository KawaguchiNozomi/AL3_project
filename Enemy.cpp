#include "Enemy.h"
#include <assert.h>
#include "Player.h"
#include "GameScene.h"
#include <cmath>

Enemy::~Enemy() {

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

		worldTransform_.UpdateMatrix();

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
		if (deathTimer < 0) {
			isDead = true;
		}
		break;
	default:
		break;
	}
	// ワールドトランスフォームの更新
	
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	
}

void Enemy::ApproachIntialize() { fireTimer_ = kFireInterval; }

void Enemy::ApproachUpdate() { 
	fireTimer_--;
	if (fireTimer_ == 0) {
		Fire();
		fireTimer_ = kFireInterval;
	}
}

Vector3 Enemy::GetWorldPosition() {
		Vector3 worldPos;
		worldPos.x = worldTransform_.translation_.x;
		worldPos.y = worldTransform_.translation_.y;
		worldPos.z = worldTransform_.translation_.z;

		return worldPos;
}

void Enemy::OnCollision() {}

void Enemy::Fire() {

	assert(player_);

	//弾の速度
	const float kBulletSpeed = 0.75f;

	//座標の取得
	Vector3 player = player_->GetWorldPosition();
	Vector3 enemy = GetWorldPosition();
	
	//敵とプレイヤーの差分ベクトル
	Vector3 vector = {(player.x - enemy.x), (player.y - enemy.y), (player.z - enemy.z)};

	//ベクトルの正規化
	float length = std::sqrtf(vector.x*vector.x + vector.y*vector.y + vector.z*vector.z);
	Vector3 dir = {vector.x / length, vector.y / length, vector.z / length};

	//長さを合わせた弾の速度
	Vector3 velocity = {dir.x * kBulletSpeed, dir.y * kBulletSpeed, dir.z * kBulletSpeed};
	EnemyBullet* bullet = new EnemyBullet();
	bullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を生成し初期化
	gameScene_->AddEnemyBullet(bullet);
	
}
