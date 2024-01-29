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
#include "Sprite.h"
#include "EnemyObstacleBullet.h"
#include <memory>
#include <list>
#include <cassert>


class Player;
///<summary>
///雑魚敵とボスの生成のまとめ
///</summary>
class EnemyManager {
public:
	EnemyManager();
	~EnemyManager();
	///<summary>
	////初期化
	///</summary>
	void Initialize(SpriteCommon* spriteCommon, Camera* camera, ParticleManager* particle);
	///<summary>
	////更新
	///</summary>
	void Update();
	///<summary>
	////ボス更新
	///</summary>
	void BossUpdate();
	///<summary>
	////描画
	///</summary>
	void Draw();
	///<summary>
	////ボス描画
	///</summary>
	void BossDraw();
	///<summary>
	////UI描画
	///</summary>
	void DrawUI();

	///<summary>
	////プレイヤーのセット
	///</summary>
	void SetPlayer(Player* player) { player_ = player; };

	///<summary>
	////ゲームシーンのセット
	///</summary>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	///<summary>
	////敵をすべて倒す
	///</summary>
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
	///<summary>
	////敵のリセット
	///</summary>
	void EnemyReset();
	///<summary>
	////雑魚敵フェーズクリアフラグ
	///</summary>
	bool Clear();
	///<summary>
	////ボスクリアフラグ
	///</summary>
	bool BossClear();
	///<summary>
	////敵の死んだときのパーティクル
	///</summary>
	void EnemyDeadParticle(Vector3 EnemyPos);
	///<summary>
	////ボスが死んだときのパーティクル
	///</summary>
	void BossDeadParticle(Vector3 EnemyPos);
	///<summary>
	////パーティクルの描画
	///</summary>
	void ParticleDraw();

	///<summary>
	////ボステスト用
	///</summary>
	void bossSeenTest();

	void ImguiUpdate();

	void CreateObstance();

public:
	//音を止める関数
	IXAudio2SourceVoice* pSourceVoice[10] = { 0 };
private://障害物
	std::list<std::unique_ptr<EnemyObstacleBullet>> enemyObstacleBullet;
		//敵の落とす弾
	Model* enemyObstacle_ = nullptr;
	Model* alertModel_ = nullptr;
	//それぞれの当たり判定
	float playerWidth = 1.5f;
	float enemyWidth_ = 0.5f;

private:
	enum Nmb
	{
		zero = 0,
		one = 1,
		two = 2,
		three = 3,
		four = 4,
		five = 5,
		six = 6,
		seven = 7,
		eight = 8,
		nine = 9,
		ten = 10,
	};
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

	Sprite* spriteRight;
	Sprite* spriteLeft;

	bool waitflag = false;
	int waitTimer = 0;
	Camera* camera_ = nullptr;
	int enemyDeath = 0;
	int clearCount = 0;
	int clearCountRis = 0;
	int clearNum = 3;

	int clearTime = 400;
	int clearTimeRis = 400;
	int randEnemyNmb;
	int randEnemyRoot;

	bool EfectEnd = false;
	int EffectTime = 50;
	int EffectTimeRis = 50;
	int EffectTimeRisEnd = 0;

	ParticleManager* enemyDeadParticle;

	//敵モデル
	Model* enemyModel_ = nullptr;
	//敵の弾モデル
	Model* enemyBulletModel_ = nullptr;


	//敵の照準モデル
	Model* enemyReticleModel_ = nullptr;

	Boss* boss = nullptr;

	int enemyParticleLife = 30;
	float enemyParticleScaleStert = 0.0f;
	float enemyParticleScaleEnd = 5.0f;

	int bossParticleLife = 40;
	float bossParticleScaleStert = 0.0f;
	float bossParticleScaleEnd = 50.0f;

	const float rnd_posS = 5.0f;
	const float rnd_velS = 0.0f;
	const float rnd_accS = 0.0000f;

	Vector3 RayPos = { 0,0,0 };

	float bossWide = 5.0f;
	float enemyWide = 3.0f;
	float enemyLockWide = 5.0f;

	float dengerTimer = 180.0f;
};