#pragma once
#include "Vector3.h"
class Collider {
public:
	float GetRadius() { return radius_; }
	void SetRadius(float radius);
	//衝突時に呼ばれる関数
	virtual void OnCollision();
	//ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;

private:
	float radius_ = 1.0f;
};
