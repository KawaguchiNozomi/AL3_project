#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include "DebugCamera.h"
#include "Enemy.h"
#include "Skydome.h"
#include "RailCamera.h"
#include "EnemyBullet.h"
#include<list>
#include <sstream>
#include "CollisionManager.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void AddEnemyBullet(EnemyBullet* enemyBullet);
	void AddEnemy(Vector3 pos);

	/// <summary>
	/// 敵発生のデータ読み込み
	/// </summary>
	void LoadEnemyPopData();

	void UpdateEnemyPopCommands();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	bool isDebugCameraActive_ = false;
	DebugCamera* debugCamera_ = nullptr;

	uint32_t textureHandle_ = 0;
	Model* model_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	Player* player_ = nullptr;

	std::list<Enemy*> enemy_;
	//EnemyBullet* enemyBullet_ = nullptr;
	std::list<EnemyBullet*> enemyBullets_;
	int32_t enemyWaitTimer_ = 0;
	bool enemyIsWait_ = false;

	Model* modelSkydome_ = nullptr;
	Skydome* skydome_ = nullptr;

	//レールカメラ
	RailCamera* railCamera_=nullptr;

	std::stringstream enemyPopCommands;

	//衝突マネージャー
	CollisionManager* collisionManager_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
