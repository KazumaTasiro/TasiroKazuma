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
#include "LightData.h"
#include "EnemyPolymo.h"

///<summary>
///敵キャラ
///</summary>
class GameScene;
class Player;
class Enemy:public EnemyPolymo {
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
	void Draw()override;
	///<summary>
	///UI描画
	///</summary>
	void DrawUI();

	///<summary>
	///描画
	///</summary>
	void ParticleDraw();

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
	////敵の登場中じゃないかｄぷか
	///</summary>
	bool GetMoveFlag(){return DemoEnemyMove;}

	void DamageParticle();

	void ImGuiUpdate();

	void NormalBulletAttck();

	void NormalBulletMove();

	void TackleAttck();

	void TackleMove();

	void ObstacleAttack();

	void ObstacleMove();

	bool ReturnObstacleFlag();

	void ResetObstacleFlag();

	float AbsoluteValue(float nmb1,float nmb2);

	void SplineMoveInitialize();

	void LightShadowOff();

private:
	float PI = 3.141592f;
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
private://弾
	//敵の弾モデル
	Model* enemyBulletModel_ = nullptr;
	//敵の照準モデル
	Model* enemyReticleModel_ = nullptr;
	//弾
	std::list<std::unique_ptr<LockOnBullet>> EnemyLockBullets_;
	std::list<std::unique_ptr<EnemyBullet>> EnemyBullets_;

	bool fireFlag = false;

	int fireTime = 45;
	int fireTimeRis = 45;

	//障害物発生フラグ
	bool obstacleFlag = false;

	float lockOnCoolTime = 25;
	float lockOnCoolTimeRe = 25;
private://敵の基本情報
	//ワールド変換データ
	Object3d* worldTransform_;
	//ワールド変換データ
	Object3d* worldTransformReticle_;
	//敵モデル
	Model* model_ = nullptr;

		////フェーズ
	//Phase phase_ = Phase::Approch;
	//キャラクターの移動ベクトル
	Vector3 ApprochMove = { 0,0,0.0f };
	Vector3 LeaveMove = { -0.1f,0.1f,-0.1f };

	Vector3 EnemyMoveSpline0 = { 0,0,0 };
	Vector3 EnemyMoveSpline1 = { -50,20,50 };
	Vector3 EnemyMoveSpline2 = { -20,15,100 };

	Vector3 EnemyReMoveSpline0 = { 0,0,0 };
	Vector3 EnemyReMoveSpline1 = { -50,30,-50 };
	Vector3 EnemyReMoveSpline2 = { -20,30,100 };

	int EnemyHp = 3;
	int EnemyHpEnd = 0;

	Vector3 EnemyScale = { 5,5,5 };
	Vector3 EnemyReticleScale = { 10,10,10 };;

	//登場時の移動状態
	bool DemoEnemyMove = false;

		//デスフラグ
	bool isDead_ = false;
	//デスフラグ
	bool isTackleDead_ = false;

	bool lockOn = false;
	float move = 0.1f;

	//タックルの情報
	float verocitySpeed = 3.0f;
	float tackPosLim = -50;



	bool TackleReMove = false;

	Vector3 velocity_;
	Vector3 velocityTackle;

	//障害物を落とすやつの基本スピード
	float obstacleEnemySpeed = 1.5f;

	Vector3 enemyResetPos = { -100,0,-30 };
private://敵のスプライン
	int EnemyRootNmb_ = 0;


	Vector3 oneEnemyMoveSpline1 = { -50,20,50 };
	Vector3 oneEnemyMoveSpline2 = { -20,15,100 };
	Vector3 oneEnemyMoveSpline0 = {  };
	Vector3 oneEnemyReMoveSpline0 = {  };
	Vector3 oneEnemyReMoveSpline1 = {  };
	Vector3 oneEnemyReMoveSpline2 = {  };

	Vector3 twoEnemyMoveSpline1 = { +50,20,50 };
	Vector3 twoEnemyMoveSpline2 = { +20,15,100 };
	Vector3 twoEnemyMoveSpline0 = {  };
	Vector3 twoEnemyReMoveSpline0 = {  };
	Vector3 twoEnemyReMoveSpline1 = { };
	Vector3 twoEnemyReMoveSpline2 = {  };

	Vector3 threeEnemyMoveSpline1 = { 10,20,-10 };
	Vector3 threeEnemyMoveSpline2 = { 10,15, -5 };
	Vector3 threeEnemyMoveSpline0 = {  };
	Vector3 threeEnemyReMoveSpline0 = {  };
	Vector3 threeEnemyReMoveSpline1 = { };
	Vector3 threeEnemyReMoveSpline2 = {  };

	Vector3 fourEnemyMoveSpline1 = { -10,20,-10 };
	Vector3 fourEnemyMoveSpline2 = { -10,15, -5 };
	Vector3 fourEnemyMoveSpline0 = {  };
	Vector3 fourEnemyReMoveSpline0 = {  };
	Vector3 fourEnemyReMoveSpline1 = { };
	Vector3 fourEnemyReMoveSpline2 = {  };

	float sprineX = 100.0f;
	float sprineY1 = 30.0f;
	float sprineY2 = 25.0f;
	float sprineZ1 = 50.0f;
	float sprineZ3 = 150.0f;
	float sprineZ2 = 100.0f;


	float Movetime = 0.05f;
	float Updatetime = 0.02f;

	SplinePosition* spline = nullptr;
	SplinePosition* splineReMove = nullptr;
private://パーティクル

	ParticleManager* particleMana_ = nullptr;

	int particleLife = 10;
	float particleScaleStert = 2.0f;
	float particleScaleEnd = 1.0f;

private://影
	int enemyNmb = 1;


	int32_t shadowNmb = 0;

	Vector4  circleShadowDir = { 0,-0.5f,0,0 };
	Vector3  circleShadowAtten = { 0,0.1f,0.0f };
	Vector2  circleShadowFactorAngle = { 0.5f,1.2f };

	bool lightActive = true;
private:

	//発射間隔
	static const int kFireInterval = 100;

	Input* input_ = nullptr;

	GameScene* gameScene_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//移動フラグ
	bool moveEndFlag = false;

	float LockWidth = 2.0f;

	int32_t time = 0;

	Player* player_ = nullptr;
	float playerWidth = 1.5f;
	float enemyWidth_ = 1.5f;
	Sprite* spriteLock = nullptr;
};