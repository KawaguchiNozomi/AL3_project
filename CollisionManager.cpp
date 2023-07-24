#include "CollisionManager.h"

void CollisionManager::ClearColliderList() { colliders_.clear(); }

void CollisionManager::SetCollider(Collider* collider) { 
	colliders_.push_back(collider); 
}

void CollisionManager::CheckAllCollisions() {

	std::list<Collider*>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		Collider* A = *itrA;
		std::list<Collider*>::iterator itrB = itrA;
		itrB++;
		for (; itrB != colliders_.end(); ++itrB) {
			Collider* B = *itrB;
			CheckCollisionPair(A, B);
		}
	}
}

void CollisionManager::CheckCollisionPair(Collider* collideA, Collider* colliderB) {
	if (collideA->GetCollisionAttribute() != colliderB->GetCollisionMask() ||
	    colliderB->GetCollisionAttribute() != collideA->GetCollisionMask()) {
		return;
	}
	Vector3 posA = collideA->GetWorldPosition();
	Vector3 posB = colliderB->GetWorldPosition();
	float distance = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
	                 (posB.z - posA.z) * (posB.z - posA.z);
	float radius = (collideA->GetRadius() + colliderB->GetRadius()) *
	               (collideA->GetRadius() + colliderB->GetRadius());
	if (distance <= radius) {
		collideA->OnCollision();
		colliderB->OnCollision();
	}
}
