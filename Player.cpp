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
	
}

void Player::Update(ViewProjection& viewProjection) {  

	viewProjection_ = viewProjection;
		bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	Vector3 move = {0,0,0};

	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}

	const float kRotSpeed = 0.02f;

	if (input_->PushKey(DIK_LEFT)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
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

	POINT mousePosition;
	//マウス座標(画面全体のスクリーン座標)を取得する
	/*画面左上から右下までの中のどこにカーソルがあるか*/
	GetCursorPos(&mousePosition);
	//クライアントエリア座標(ウィンドウ内のスクリーン座標)に変換する
	/*今開いているウィンドウの中のどこにカーソルがあるか*/
	HWND hwnd = WinApp::GetInstance()->GetHwnd();
	ScreenToClient(hwnd, &mousePosition);
	//2Dレティクルに
	sprite2DRetecle_->SetPosition(Vector2{float(mousePosition.x),float(mousePosition.y)});
	//ビュープロジェクションビューポート合成行列
	Matrix4x4 matVPV = viewProjection_.matView * viewProjection_.matProjection *
	                   MakeViewportMatrix(0, 0, 1280, 720, 0, 1);
	//合成行列の逆行列を計算する
	Matrix4x4 matInveersVPV = Inverse(matVPV);
	//スクリーン座標
	Vector3 posNear = Vector3(float(mousePosition.x), float(mousePosition.y), 0);
	Vector3 posFar = Vector3(float(mousePosition.x), float(mousePosition.y), 1);
	//スクリーン座標系からワールド座標系へ
	posNear = Transform(posNear, matInveersVPV);
	posFar = Transform(posFar, matInveersVPV);

	//マウスレイの方向
	Vector3 mouseDirection = posFar-posNear;
	mouseDirection = Normalize(mouseDirection);
	//カメラから照準オブジェクトの距離
	const float kDistanceTestObject =  50;
	worldTransform3DReticle_.translation_ = posNear + mouseDirection * kDistanceTestObject;

	worldTransform3DReticle_.UpdateMatrix();
	worldTransform3DReticle_.TransferMatrix();


	/*
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
	*/


	//キャラクター攻撃処理
	fireTimer_--;
	Attack();
	//弾を更新する
	for (PlayerBullet*bullet : bullets_) {
		bullet->Update();
	}

	//ImGui
#ifdef _DEBUG
	ImGui::SetNextWindowSize({250,250});
	ImGui::Begin("Player");
	float sliderValue[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	ImGui::SliderFloat3("pos", sliderValue, -20.0f, 20.0f);
	ImGui::Text(
	    "pos::(%f,%f,%f)", worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z);
	float reticle[3] = {worldTransform3DReticle_.translation_.x,worldTransform3DReticle_.translation_.y,worldTransform3DReticle_.translation_.z};
	ImGui::InputFloat3("reticle", reticle);
	worldTransform_.translation_ = {sliderValue[0], sliderValue[1], sliderValue[2]};
	ImGui::Text(
	    "2DReticle:(%f,%f)", sprite2DRetecle_->GetPosition().x, sprite2DRetecle_->GetPosition().y);
	ImGui::Text("Near:(%+.2f,&+.2f,%+.2f)", posNear.x, posNear.y, posNear.z);
	ImGui::Text("Far:(%+.2f,&+.2f,%+.2f)", posFar.x, posFar.y, posFar.z);
	ImGui::Text(
	    "3DRetice:(%+.2f,%+.2f,%+.2f)", worldTransform3DReticle_.translation_.x,
	    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
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

Vector3 Player::GetReticleWorldPos() {

	Vector3 worldPos;
	worldPos.x = worldTransform3DReticle_.matWorld_.m[3][0];
	worldPos.y = worldTransform3DReticle_.matWorld_.m[3][1];
	worldPos.z = worldTransform3DReticle_.matWorld_.m[3][2];

	return worldPos;
}

void Player::OnCollision() {}

void Player::Attack() {
	if (fireTimer_ <= 0) {
		if (input_->PushKey(DIK_SPACE)) {

			// 弾の速度
			const float kBulletSpeed = 1.0f;
			// Vector3 velocity(0, 0, kBulletSpeed);
			// velocity = TransforNomal(velocity, worldTransform_.matWorld_);
			//
			// 3Dレティクルと自機の差分ベクトル
			Vector3 velocity = GetReticleWorldPos() - GetWorldPosition();
			velocity = Normalize(velocity) * kBulletSpeed;
			// 弾を生成し初期化
			PlayerBullet* newBullet = new PlayerBullet();
			newBullet->Initialize(model_, GetWorldPosition(), velocity);
			// 弾を登録する
			bullets_.push_back(newBullet);
			fireTimer_ = kFireInterval;
		}
	}
}

void Player::SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

void Player::DrawUI() { sprite2DRetecle_->Draw(); }
