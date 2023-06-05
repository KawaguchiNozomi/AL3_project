#include "Skydome.h"
#include <assert.h>

void Skydome::Initialize(Model* model) {
	assert(model);
	model_ = model;
	// シングルトンインスタンスを取得
	//input_ = Input::GetInstance();

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0,0,0};
	//textureHandle_ = textureHandle;
}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}
