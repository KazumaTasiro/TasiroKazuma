#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>
#include "Input.h"
#include <memory>
#include <list>
#include "Vector3.h"
#include "LockOnBullet.h"
#include "Sprite.h"
#include "Collision.h"
#include "SplinePosition.h"
#include "EnemyBullet.h"
#include "ParticleManager.h"

class GameScene;
class Player;
///<summary>
///ボスの行動
///</summary>
class Boss
{
public:
	Boss();
	~Boss();

	///<summary>
	///初期化
	///</summary>
	void Initialize(Model* enemyBulletModel, Model* enemyReticleModel);
	///<summary>
	///更新
	///</summary>

	void Update(Player* player);
	///<summary>
	////ボスの行動
	///</summary>
	void Move();


	//void Fire();
	///<summary>
	////座標
	///</summary>
	Vector3 GetWorldPosition();
	///<summary>
	///描画
	///</summary>
	void Draw();
	///<summary>
	///描画
	///</summary>
	void ParticleDraw();
	///<summary>
	////ゲームシーンの設定
	///</summary>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	///<summary>
	///ボス消滅フラグ
	///</summary>
	bool IsDead()const { return isDead_; }
	///<summary>
	///ロックオンのコールバック関数
	///</summary>
	void LockOnTrue();
	///<summary>
	////衝突を検出する関数
	///</summary>
	void OnColl();

	///<summary>
	////衝突を検出したら呼び出されるコールバック関数
	///</summary>
	void OnCollision();
	///<summary>
	////リセット関数
	///</summary>
	void Reset();
	///<summary>
	///ボスの弾発射関数
	///</summary>
	void Fire();

	void DamageParticle();
private:

	int randBossAttackNmb = 0;

	//発射間隔
	static const int kFireInterval = 100;

	Input* input_ = nullptr;

	GameScene* gameScene_ = nullptr;

	//ワールド変換データ
	Object3d* worldTransform_;
	//ワールド変換データ
	Object3d* worldTransformReticle_;
	//敵モデル
	Model* model_ = nullptr;
	//敵の弾モデル
	Model* enemyBulletModel_ = nullptr;
	//敵の照準モデル
	Model* enemyReticleModel_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	////フェーズ
	//Phase phase_ = Phase::Approch;
	//キャラクターの移動ベクトル
	Vector3 ApprochMove = { 0,0,0.0f };
	Vector3 LeaveMove = { -0.1f,0.1f,-0.1f };

	Vector3 EnemyMoveSpline0 = { 0,0,0 };
	Vector3 EnemyMoveSpline0Ris = { 0,50,200 };
	Vector3 EnemyMoveSpline1 = { 0,150,200 };
	Vector3 EnemyMoveSpline2 = { 0,100,200 };

	//弾
	std::list<std::unique_ptr<LockOnBullet>> EnemyLockBullets_;
	std::list<std::unique_ptr<EnemyBullet>> EnemyBullets_;

	int32_t time = 0;

	Player* player_ = nullptr;

	int EnemyHp = 10;

	//デスフラグ
	bool isDead_ = false;

	bool lockOn = false;
	float move = 0.1f;

	Sprite* spriteLock = nullptr;

	SplinePosition* spline = nullptr;
	bool DemoEnemyMove = false;

	float PI = 3.141592f;

	bool fireFlag = false;

	int fireTime = 45;
	int fireTimeRisSm = 35;
	int fireTimeRisBig = 70;

	float fireCount = 0.0f;
	float fireCountMax = 4.0f;
	float fireCountSm = 0.5f;
	float fireCountBig = 1.0f;

	float verocitySpeed = 4.0f;
	float bigVerocitySpeed = 3.0f;
	
	Vector3 PosRis = { 100,200,200 };

	float AttackSm = 1.5f;
	float AttackBig = 1.5f;
	float BossColl = 50.0f;

	Vector3 velocity_;

	Vector3 ReticleSc = { 70,70,70 };

	Vector3 worldRt = { 0,( 89 / PI ),0 };
	Vector3 worldSc = { 50,50,50 };
	Vector3 worldPs = { 50,200,200 };

	float time_ = 0.02f;

	Vector3 BulletSize = { 4.0f,4.0f,4.0f };

	ParticleManager* particleMana_ = nullptr;

	int particleLife = 10;
	float particleScaleStert = 15.0f;
	float particleScaleEnd = 1.0f;
};
