#pragma once
#include "Vector3.h"
#include <cstdint>
#include "CollisionConfig.h"
class Collider {
public:
	float GetRadius() { return radius_; }
	void SetRadius(float radius);
	//衝突時に呼ばれる関数
	virtual void OnCollision();
	//ワールド座標を取得
	virtual Vector3 GetWorldPosition() = 0;

	int GetCollisionAttribute() { return collisionAttribute_; }
	void SetCollisionAttribute(uint32_t attribute);
	int GetCollisionMask() { return CollisionMask_; }
	void SetCollisionMask(uint32_t mask);

private:
	float radius_ = 1.0f;
	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0xffffffff;
	//衝突マスク(相手) 当たりたいやつをいれる
	uint32_t CollisionMask_ = 0xffffffff;

	
};
