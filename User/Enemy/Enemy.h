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

///<summary>
///敵キャラ
///</summary>
class GameScene;
class Player;
class Enemy {
public:
	///<summary>
	///初期化
	///</summary>
	/// EnemyNmb 敵の行動の種類
	/// EnemyRootNmb 敵の行動ルート　0:左 1:右
	void Initialize(Vector3 EnemyPos, SpriteCommon* sptriteCommon, Model* model, Model* enemyBulletModel, Model* enemyReticleModel,int EnemyNmb, int EnemyRootNmb);
	///<summary>
	///更新
	///</summary>

	void Update(Player* player);
	///<summary>
	////敵の動き
	///</summary>
	void Move();

	///<summary>
	////敵の間の発射
	///</summary>
	void Fire();
	///<summary>
	////敵の座標
	///</summary>
	Vector3 GetWorldPosition();
	///<summary>
	///描画
	///</summary>
	void Draw();
	///<summary>
	///UI描画
	///</summary>
	void DrawUI();

	////行動フェーズ
	//enum class Phase {
	//	Approch,//接近する
	//	Leave,//離脱する
	//};
	//void Approch();

	//void Leave();


	////ベクトルを正規化する
	//int Vec3Normalize(Vector3* pOut, Vector3* pV);

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	////弾リストを取得
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets2_; }
	///<summary>
	////ゲームシーンのセット
	///</summary>
	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }
	///<summary>
	////死亡判定
	///</summary>
	bool IsDead()const { return isDead_; }
	///<summary>
	////タックルで死んだときの判定
	///</summary>
	bool IsTackleDead()const { return isTackleDead_; }
	///<summary>
	////ロックオンをオンにする
	///</summary>
	void LockOnTrue();
	///<summary>
	////プレイヤーの設定
	///</summary>
	void setPlayer(Player* player) { player_ = player; }
	///<summary>
	////ロックオンの弾の当たり判定
	///</summary>
	void OnColl();

	///<summary>
	////敵を倒すと1を返す関数
	///</summary>
	int ReturnOnColl();


	///<summary>
	////弾リストを取得
	///</summary>
	const std::list<std::unique_ptr<LockOnBullet>>& GetBullets() { return EnemyLockBullets_; }

	///<summary>
	////タックルの衝突反応
	///</summary>
	void CollTackle();
	///<summary>
	////敵の登場中じゃないか
	///</summary>
	bool GetMoveFlag(){return DemoEnemyMove;}

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

	Vector3 EnemyMoveSpline0 = { 0,0,0};
	Vector3 EnemyMoveSpline1 = { -50,20,50 };
	Vector3 EnemyMoveSpline2 = { -20,15,100 };

	Vector3 EnemyReMoveSpline0 = { 0,0,0 };
	Vector3 EnemyReMoveSpline1 = { -50,30,-50 };
	Vector3 EnemyReMoveSpline2 = { -20,30,100 };

	//移動フラグ
	bool moveEndFlag = false;

	//弾
	std::list<std::unique_ptr<LockOnBullet>> EnemyLockBullets_;
	std::list<std::unique_ptr<EnemyBullet>> EnemyBullets_;

	int32_t time = 0;

	Player* player_ = nullptr;

	int EnemyHp = 1;

	//デスフラグ
	bool isDead_ = false;
	//デスフラグ
	bool isTackleDead_ = false;

	bool lockOn = false;
	float move = 0.1f;

	Sprite* spriteLock = nullptr;

	SplinePosition* spline = nullptr;
	SplinePosition* splineReMove = nullptr;
	bool DemoEnemyMove = false;

	bool fireFlag = false;

	int fireTime = 45;

	float verocitySpeed = 3.0f;

	int enemyNmb = 1;

	bool TackleReMove = false;

	Vector3 velocity_;
	Vector3 velocityTackle;

	int EnemyRootNmb_ = 0;
};