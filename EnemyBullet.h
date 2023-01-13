#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>
#include "Input/Input.h"


///<summary>
///自キャラ
///</summary>
class EnemyBullet {
public:
	///<summary>
	///初期化
	///</summary>

	void Initialize( const XMFLOAT3& position, const XMFLOAT3& velocity);;



	///<summary>
	///更新
	///</summary>

	void Update();

	///<summary>
	///描画
	///</summary>

	void Draw();

	bool IsDead()const { return isDead_; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	XMFLOAT3 GetWorldPosition();

private:
	//ワールド変換データ
	Object3d* worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	XMFLOAT3 velocity_;

	//寿命<fim>
	static const int32_t kLifeTime = 60 * 10000;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;
};
