#pragma once
#include"Collider.h"
#include<list>
class CollisionManager {
public:
	//コライダーリストをクリアする
	void ClearColliderList();
	//リストに登録
	void SetCollider(Collider* collider);
	//当たり判定の処理
	void CheckAllCollisions();

private:
	void CheckCollisionPair(Collider* collideA, Collider* colliderB);
	std::list<Collider*> colliders_;
};
