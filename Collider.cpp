#include "Collider.h"

void Collider::SetRadius(float radius) { radius_ = radius; }

void Collider::OnCollision() {}

void Collider::SetCollisionAttribute(uint32_t attribute) { collisionAttribute_ = attribute; }

void Collider::SetCollisionMask(uint32_t mask) { CollisionMask_ = mask; }
