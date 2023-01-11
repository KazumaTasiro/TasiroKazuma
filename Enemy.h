#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>
#include "Input/Input.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include <EnemyBullet.h>



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
	void Initialize(Model* model, uint32_t textureHandle, XMFLOAT3 EnemyPos);
	///<summary>
	///更新
	///</summary>

	void Update();

	void Move();


	void Fire();

	XMFLOAT3 GetWorldPosition();
	///<summary>
	///描画
	///</summary>
	void Draw();
	//行動フェーズ
	enum class Phase {
		Approch,//接近する
		Leave,//離脱する
	};
	void Approch();

	void Leave();

	void SetPlayer(Player* player) { player_ = player; }

	//ベクトルを正規化する
	int Vec3Normalize(XMFLOAT3* pOut, XMFLOAT3* pV);

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	////弾リストを取得
	//const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return bullets2_; }

	void SetGameScene(GameScene* gameScene) { gameScene_ = gameScene; }

	bool IsDead()const { return isDead_; }

private:
	//発射間隔
	static const int kFireInterval = 100;

	Player* player_ = nullptr;

	GameScene* gameScene_ = nullptr;

	//ワールド変換データ
	Object3d worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//モデル
	Model* enemyBulletModel_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//フェーズ
	Phase phase_ = Phase::Approch;
	//キャラクターの移動ベクトル
	XMFLOAT3 ApprochMove = { 0,0,0.0f };
	XMFLOAT3 LeaveMove = { -0.1f,0.1f,-0.1f };


	int32_t time = 0;

	int EnemyHp = 1;

	//デスフラグ
	bool isDead_ = false;
};
