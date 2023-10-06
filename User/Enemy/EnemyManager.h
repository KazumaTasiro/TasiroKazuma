#pragma once
#include "DirectXCommon.h"
#include "PlayerBullet.h"
#include "Collision.h"
#include "Enemy.h"
#include "Audio.h"
#include "Input.h"
#include "SpriteCommon.h"
#include "LockOnBullet.h"
#include "ParticleManager.h"
#include "Boss.h"


class Player;

class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();

	void Initialize( Input* input, SpriteCommon* spriteCommon, Camera* camera, ParticleManager* particle);

	void Update();

	void BossUpdate();

	void Draw();

	void BossDraw();

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

	bool BossClear();

	void EnemyDeadParticle(Vector3 EnemyPos);

	void BossDeadParticle(Vector3 EnemyPos);

	void ParticleDraw();
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
	int clearNum = 5;

	int clearTime = 400;
	int randEnemyNmb;
	int randEnemyRoot;

	bool EfectEnd = false;
	int EffectTime = 50;

	ParticleManager* enemyDeadParticle;

	//敵モデル
	Model* enemyModel_ = nullptr;
	//敵の弾モデル
	Model* enemyBulletModel_ = nullptr;
	//敵の照準モデル
	Model* enemyReticleModel_ = nullptr;

	Boss* boss = nullptr;
};