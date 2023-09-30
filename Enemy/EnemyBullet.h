#pragma once
#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>
#include "Input.h"


///<summary>
///自キャラ
///</summary>
class EnemyBullet {
public:
	///<summary>
	///初期化
	///</summary>

	void Initialize(const Vector3& position, const Vector3& velocity, Model* bulletModel);



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

	Vector3 GetWorldPosition();

	void SetSize(Vector3 Size) { worldTransform_->wtf.scale = Size; }


private:
	//ワールド変換データ
	Object3d* worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//速度
	Vector3 velocity_;

	//寿命<fim>
	static const int32_t kLifeTime = 60 * 1000;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//デスフラグ
	bool isDead_ = false;
};