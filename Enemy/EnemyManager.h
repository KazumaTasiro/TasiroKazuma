#pragma once
#include "DirectXCommon.h"
#include "PlayerBullet.h"
#include "Collision.h"
#include "Enemy.h"
#include "Audio.h"
#include "Input.h"
#include "SpriteCommon.h"
#include "LockOnBullet.h"

class Player;

class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();

	void Initialize(DirectXCommon* dxCommon, Input* input, SpriteCommon* spriteCommon, Camera* camera);

	void Update();

	void Draw();

	void DrawUI();


	void SetPlayer(Player* player) { player_ = player; };

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsAllEnemyDead();

	/// <summary>
	/// 敵のCSVの読み込み
	/// </summary>
	void LoadEnemyPopData();

	//CSVリセット
	void EnemyPopComandReset();

	/// <summary>
	/// 敵のCSVの解析
	/// </summary>
	void UpdateEnemyPopCommands();

	//敵の登録
	void ExistenceEnemy(const Vector3& EnemyPos);

	//敵との当たり判定
	void EnemyCollision(Player* player);

	void EnemyReset();

	bool Clear();
public:
	//音を止める関数
	IXAudio2SourceVoice* pSourceVoice[10] = { 0 };

private:
	DirectXCommon* dxCommon = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;
	Audio* audio = nullptr;
	Transform* origin = nullptr;

	GameScene* gameScene_ = nullptr;

	Player* player_ = nullptr;

	Input* input_ = nullptr;

	std::list<std::unique_ptr<Enemy>> enemy_;

	//敵発生コマンド
	std::stringstream enemyPopCommands;

	bool waitflag = false;
	int waitTimer = 0;
	Camera* camera_ = nullptr;
	int clearCount = 0;
	int clearNum = 3;
};