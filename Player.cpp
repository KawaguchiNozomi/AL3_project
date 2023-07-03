#include "Player.h"
#include "MathUtility.h"

Player::~Player() {
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	delete sprite2DRetecle_;
}

void Player::Initialize(Model* model, uint32_t textureHandle,Vector3 position) { 
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	//シングルトンインスタンスを取得
	input_ = Input::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform3DReticle_.Initialize();
	uint32_t textureReticle = TextureManager::Load("target.png");
	sprite2DRetecle_ = Sprite::Create(
	    textureReticle, {640, 360}, {1,1,1,1},
	    {0.5, 0.5});
	viewProjection_.Initialize();
	
}

void Player::Update() {  

		bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	Vector3 move = {0,0,0};

	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	const float kRotSpeed = 0.02f;

	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}

	//範囲制限
	const float kMoveLimitX = 20.0f;
	const float kMoveLimitY = 20.0f;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.translation_.x += move.x;
	worldTransform_.translation_.y += move.y;
	worldTransform_.translation_.z += move.z;

	worldTransform_.UpdateMatrix();

	worldTransform_.TransferMatrix();

	//自機のワールド座標から3Dレティクルのワールド座標を計算
	//自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 50.0f;
	//自機から3Dレティクルへのオフセット（Z+向き）
	Vector3 offset = {0, 0, 1.0f};
	offset = TransforNomal(offset, worldTransform_.matWorld_);
	offset = Normalize(offset)*kDistancePlayerTo3DReticle;
	worldTransform3DReticle_.translation_ = TransforNomal(worldTransform_.translation_,worldTransform_.matWorld_);
	worldTransform3DReticle_.UpdateMatrix();
	//worldTransform3DReticle_.TransferMatrix();

	Vector3 positionReticle = worldTransform3DReticle_.translation_;
	Matrix4x4 matViewport = MakeViewportMatrix(0, 0, 1280, 720, 0, 1);
	Matrix4x4 matViewProjectionViewPort =
	    viewProjection_.matView * viewProjection_.matProjection * matViewport;
	positionReticle = Transform(positionReticle, matViewProjectionViewPort);
	sprite2DRetecle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));



	//キャラクター攻撃処理
	Attack();
	//弾を更新する
	for (PlayerBullet*bullet : bullets_) {
		bullet->Update();
	}

	//ImGui
#ifdef _DEBUG
	ImGui::SetNextWindowSize({200,150});
	ImGui::Begin("Player");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("pos", sliderValue, -20.0f, 20.0f);
	float reticle[3] = {worldTransform3DReticle_.translation_.x,worldTransform3DReticle_.translation_.y,worldTransform3DReticle_.translation_.z};
	ImGui::InputFloat3("reticle", reticle);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::End();
#endif // DEBUG
}

void Player::Draw(ViewProjection& viewProjection) { 
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	for (PlayerBullet*bullet : bullets_) {
		bullet->Draw(viewProjection);
	}

}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() {}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {
		
		//弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		velocity = TransforNomal(velocity, worldTransform_.matWorld_);
		//Vector3 velocity = {
		//    (worldTransform3DReticle_.translation_.x - worldTransform_.translation_.x),
		//    (worldTransform3DReticle_.translation_.y - worldTransform_.translation_.y),
		//    (worldTransform3DReticle_.translation_.z - worldTransform_.translation_.z)};
		//velocity = Normalize(velocity) * kBulletSpeed;
		//弾を生成し初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		//弾を登録する
		bullets_.push_back(newBullet);
	}
}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

void Player::DrawUI() { sprite2DRetecle_->Draw(); }
