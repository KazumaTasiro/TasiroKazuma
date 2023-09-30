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

	void Initialize(DirectXCommon* dxCommon, Input* input, SpriteCommon* spriteCommon, Camera* camera, ParticleManager* particle);

	void Update();

	void BossUpdate();

	void Draw();

	void BossDraw();

	void DrawUI();


	void SetPlayer(Player* player) { player_ = player; };

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsAllEnemyDead();

	/// <summary>
	/// “G‚ÌCSV‚Ì“Ç‚İ‚İ
	/// </summary>
	void LoadEnemyPopData();

	//CSVƒŠƒZƒbƒg
	void EnemyPopComandReset();

	/// <summary>
	/// “G‚ÌCSV‚Ì‰ğÍ
	/// </summary>
	void UpdateEnemyPopCommands();

	//“G‚Ì“o˜^
	void ExistenceEnemy(const Vector3& EnemyPos);

	//“G‚Æ‚Ì“–‚½‚è”»’è
	void EnemyCollision(Player* player);

	void EnemyReset();

	bool Clear();

	bool BossClear();

	void EnemyDeadParticle(Vector3 EnemyPos);

	void BossDeadParticle(Vector3 EnemyPos);

	void ParticleDraw();
public:
	//‰¹‚ğ~‚ß‚éŠÖ”
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

	//“G”­¶ƒRƒ}ƒ“ƒh
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

	//“Gƒ‚ƒfƒ‹
	Model* enemyModel_ = nullptr;
	//“G‚Ì’eƒ‚ƒfƒ‹
	Model* enemyBulletModel_ = nullptr;
	//“G‚ÌÆ€ƒ‚ƒfƒ‹
	Model* enemyReticleModel_ = nullptr;

	Boss* boss = nullptr;
};