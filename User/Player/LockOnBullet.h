#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>
#include "Input.h"
#include "Vector3.h"
#include "SplinePosition.h"


///<summary>
///プレイヤーのロックオンの弾
///</summary>
class LockOnBullet
{
public:
	LockOnBullet();
	~LockOnBullet();

	///<summary>
	///初期化
	///</summary>
	void Initialize(Model* model, const Vector3& position);
	///<summary>
	///更新
	///</summary>
	void Update(const Vector3& enemyPos);
	///<summary>
	///描画
	///</summary>
	void Draw();

	///<summary>
	///弾の消滅フラグ
	///</summary>
	bool IsDead()const { return isDead_; }

	
	///<summary>
	////衝突を検出したら呼び出されるコールバック関数
	///</summary>
	void OnCollision();
	///<summary>
	////弾の座標
	///</summary>
	Vector3 GetWorldPosition();
private:
	//ワールド変換データ
	Object3d* worldTransform_;

	//モデル
	Model* model_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	float speed = 3.0f;

	//速度
	Vector3 velocity_;
	//寿命<fim>
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;

	float time = 0;

	Vector3 worldNormal;
};
