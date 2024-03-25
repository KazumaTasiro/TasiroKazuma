#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>
#include "Input.h"

///<summary>
///雑魚敵の行動
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

	///<summary>
	////弾消滅
	///</summary>
	bool IsDead()const { return isDead_; }

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	///<summary>
	////敵の座標
	///</summary>
	Vector3 GetWorldPosition();

	///<summary>
	////弾のサイズを変える
	///</summary>
	void SetSize(Vector3 Size) { worldTransform_->wtf.scale = Size; }

	///<summary>
	////弾の進行方向に弾に向きを変える
	///</summary>
	void VecRot();

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
	int32_t deathTimerEnd_ = 0;
	//デスフラグ
	bool isDead_ = false;
};