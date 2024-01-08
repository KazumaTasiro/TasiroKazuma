#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>
#include "Input.h"
#include "Vector3.h"

class EnemyObstacleBullet
{
public:
	EnemyObstacleBullet();
	~EnemyObstacleBullet();
	///<summary>
	///初期化
	///</summary>
	void Initialize(Model* model,const Vector3& position,const Vector3& velocity);
	///<summary>
	///更新
	///</summary>
	void Update();
	///<summary>
	///描画
	///</summary>
	void Draw();
	///<summary>
	///弾の消滅フラグ
	///</summary>
	bool IsDead()const {
		return isDead_;
	}

///<summary>
////衝突を検出したら呼び出されるコールバック関数
///</summary>
	void OnCollision();
private:
		//ワールド変換データ
	Object3d* worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//弾の速度
	float speed = 3.0f;

	//速度
	Vector3 velocity_;
	//寿命<fim>
	static const int32_t kLifeTime = 60 * 3;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
		//デスタイマー
	int32_t deathTimerEnd_ = 0;
	//デスフラグ
	bool isDead_ = false;

	Vector3 worldSc = { 1,1,1 };
};