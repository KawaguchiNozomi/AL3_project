#pragma once
#include <cstdint>
class CollisionConfig {
public:
	//プレイヤーの
	static const uint32_t kCollisionAttributePlayer_ = 0b1;
	//敵
	static const uint32_t kCollisionAttributeEnemy_ = 0b1 << 1;
};
