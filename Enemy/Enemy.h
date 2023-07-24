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
	void Initialize(Vector3 EnemyPos, Input* input,SpriteCommon* sptriteCommon);
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
	///<summary>
	///描画
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

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead()const { return isDead_; }

	void LockOnTrue();

	void setPlayer(Player* player_) { player = player_; }

	void OnColl();

	//弾リストを取得
	const std::list<std::unique_ptr<LockOnBullet>>& GetBullets() { return EnemyLockBullets_; }

private:
	//発射間隔
	static const int kFireInterval = 100;

	Input* input_ = nullptr;

	GameScene* gameScene_ = nullptr;

	//ワールド変換データ
	Object3d* worldTransform_;
	//ワールド変換データ
	Object3d* worldTransformReticle_;
	//モデル
	Model* model_ = nullptr;
	//モデル
	Model* enemyBulletModel_ = nullptr;
	//モデル
	Model* enemyReticleModel_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	////フェーズ
	//Phase phase_ = Phase::Approch;
	//キャラクターの移動ベクトル
	Vector3 ApprochMove = { 0,0,0.0f };
	Vector3 LeaveMove = { -0.1f,0.1f,-0.1f };

	//弾
	std::list<std::unique_ptr<LockOnBullet>> EnemyLockBullets_;

	int32_t time = 0;

	Player* player = nullptr;

	int EnemyHp = 1;

	//デスフラグ
	bool isDead_ = false;

	bool lockOn = false;
	float move = 0.1f;

	Sprite* spriteLock = nullptr;
};