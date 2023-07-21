#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>
#include "AxisIndicator.h"
#include <fstream>

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;
	delete player_;
	delete debugCamera_;
	delete modelSkydome_;
	delete skydome_;
	delete railCamera_;
	for (EnemyBullet* bullet : enemyBullets_) {
		delete bullet;
	}
	for (Enemy* enemy : enemy_) {
		delete enemy;
	}
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	debugCamera_ = new DebugCamera(50, 50);

	textureHandle_ = TextureManager::Load("sample.png");
	model_ = Model::Create();

	worldTransform_.Initialize();
	viewProjection_.Initialize();
	//	自キャラ生成
	player_ = new Player();
	Vector3 playerPosition(0, 0, 30.0f);
	//自キャラ初期化
	player_->Initialize(model_,textureHandle_,playerPosition);

	//軸方向表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向が参照するビュープロジェクションを指定する(アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	

	//天球の初期化、生成
	skydome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_->Initialize(modelSkydome_);

	//レールカメラの初期化
	railCamera_ = new RailCamera();
	railCamera_->Initialize(worldTransform_.translation_,worldTransform_.rotation_);
	
	player_->SetParent(&railCamera_->GetWorldTransform());
	//AddEnemy({0.0f, 5.0f, 30.0f});
	LoadEnemyPopData();

	TextureManager::Load("target.png");
}

void GameScene::Update() { 

	skydome_->Update();
	// プレイヤー更新
	player_->Update();
	
	//敵ポップ
	UpdateEnemyPopCommands();
	//敵更新
	enemy_.remove_if([](Enemy * enemy) {
		if (enemy->GetIsDead()) {
			delete enemy;
			return true; //←returnをつける
		}
		return false;
	});
	for (Enemy* enemy : enemy_) {
		enemy->Update();
	}

	// 敵弾
	enemyBullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Update();
	}

	CheckAllCollisions();

	if (isDebugCameraActive_ == false) {
		railCamera_->Update();
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}
	#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = true;
	}
#endif // _DEBUG
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクションの行列転送
		viewProjection_.TransferMatrix();
		if (input_->TriggerKey(DIK_9)) {
			isDebugCameraActive_ = false;
		}
	} else {
		//viewProjection_.UpdateMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	//天球描画
	skydome_->Draw(viewProjection_);
	//プレイヤー描画
	player_->Draw(viewProjection_);
	//敵描画
	for (Enemy* enemy : enemy_) {
		enemy->Draw(viewProjection_);
	}
	for (EnemyBullet* bullet : enemyBullets_) {
		bullet->Draw(viewProjection_);
	}
	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	player_->DrawUI();
	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() { 
	//判定対象AとBの座標
	Vector3 posA, posB;

    //プレイヤーの弾リスト
    //
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
    //敵の弾リスト
	const std::list<EnemyBullet*>& enemyBullets = enemyBullets_;

	//const std::list<Enemy*>& enemy = enemy_;

    #pragma region
    posA = player_->GetWorldPosition();
    
    for (EnemyBullet* bullet : enemyBullets) {
    
    	posB = bullet->GetWorldPosition();
    	float distance = (posB.x - posA.x) * (posB.x - posA.x) +
    	                 (posB.y - posA.y) * (posB.y - posA.y) +
    	                 (posB.z - posA.z) * (posB.z - posA.z);
    	float radius =
    		(player_->GetRadius() + bullet->GetRadius()) * (player_->GetRadius() + bullet->GetRadius());
    
		if (distance <= radius) {
			player_->OnCollision();
			bullet->OnCollision();
		}
    }
	#pragma endregion

	#pragma region
	for (Enemy* enemy : enemy_) {
		posA = enemy->GetWorldPosition();
		for (PlayerBullet* bullet : playerBullets) {
			posB = bullet->GetWorldPosition();

			float distance = (posB.x - posA.x) * (posB.x - posA.x) +
			                 (posB.y - posA.y) * (posB.y - posA.y) +
			                 (posB.z - posA.z) * (posB.z - posA.z);
			float radius = (enemy->GetRadius() + bullet->GetRadius()) *
			               (enemy->GetRadius() + bullet->GetRadius());

			if (distance <= radius) {
				enemy->OnCollision();
				bullet->OnCollision();
			}
		}
	}
	#pragma endregion

	#pragma region
	for (PlayerBullet* bulletP : playerBullets) {
		posA = bulletP->GetWorldPosition();
		for (EnemyBullet* bulletE : enemyBullets) {
			posB = bulletE->GetWorldPosition();
			float distance = (posB.x - posA.x) * (posB.x - posA.x) +
			                 (posB.y - posA.y) * (posB.y - posA.y) +
			                 (posB.z - posA.z) * (posB.z - posA.z);
			float radius = (bulletP->GetRadius() + bulletE->GetRadius()) *
			               (bulletP->GetRadius() + bulletE->GetRadius());

			if (distance <= radius) {
				bulletP->OnCollision();
				bulletE->OnCollision();
			}
		}
	}
}

void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {

	enemyBullets_.push_back(enemyBullet); }

void GameScene::AddEnemy( Vector3 pos) {
	// 敵キャラの生成
	Enemy* obj = new Enemy();
	// 敵キャラの初期化
	obj->Initialize(model_, pos);
	obj->SetPlayer(player_);
	obj->SetGameScene(this);
	enemy_.push_back(obj);
}

void GameScene::LoadEnemyPopData() { 
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/enemyPop.csv");
	assert(file.is_open());

	enemyPopCommands << file.rdbuf();
	//ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemyPopCommands() {
	//待機処理
	if (enemyIsWait_) {
		enemyWaitTimer_--;
		if (enemyWaitTimer_ <= 0) {
			enemyIsWait_ = false;
		}
		//↓忘れるとこの関数で止まってしまう
		return;
	}

	//一行分の文字を入れる変数
	std::string line;
	while (getline(enemyPopCommands,line)) {
		//一行分の文字列をストリーム変換して解析しやすくする
		std::istringstream line_stream(line);

		std::string word;
		//,区切りで行の先頭文字を取得
		getline(line_stream, word, ',');
		//コメント行を飛ばす
		if (word.find("//") == 0) {
			continue;
		}
		//POPコマンド
		if (word.find("POP") == 0) {
			//x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());
			//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());
			//z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());
			AddEnemy(Vector3(x, y, z));
		}
		//WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');
			//待ち時間
			int32_t waitTime = atoi(word.c_str());
			//待機開始
			enemyIsWait_ = true;
			enemyWaitTimer_ = waitTime;
			//コマンドループを抜ける
			break;
		} 
	}
}
