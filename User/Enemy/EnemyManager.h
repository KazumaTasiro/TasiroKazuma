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
	void Initialize( Input* input, SpriteCommon* spriteCommon, Camera* camera, ParticleManager* particle);
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