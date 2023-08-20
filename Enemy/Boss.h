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


class GameScene;
class Player;
class Boss
{
public:
	Boss();
	~Boss();

	///<summary>
	///初期化
	///</summary>
	void Initialize(Model* enemyBulletModel, Model* enemyReticleModel, Input* input);
	///<summary>
	///更新
	///</summary>

	void Update(Player* player);

	void Move();


	//void Fire();

	Vector3 GetWorldPosition();
	///<summary>
	///描画
	///</summary>
	void Draw();

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead()const { return isDead_; }

	void LockOnTrue();

	void OnColl();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	void Reset();

	bool isDead();

	void Fire();
private:

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
	Vector3 EnemyMoveSpline1 = { 0,150,200 };
	Vector3 EnemyMoveSpline2 = { 0,100,200 };

	//弾
	std::list<std::unique_ptr<LockOnBullet>> EnemyLockBullets_;
	std::list<std::unique_ptr<EnemyBullet>> EnemyBullets_;

	int32_t time = 0;

	Player* player = nullptr;

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

	float verocitySpeed = 3.0f;

	Vector3 velocity_;
};
