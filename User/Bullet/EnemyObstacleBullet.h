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
	void Initialize(Model* model,Model* alertRoadModel,const Vector3& position);
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

	Vector3 GetWorldPosition();

	void GravityUpdate();

	void RoadUpdate();

private:
	//ワールド変換データ
	Object3d* worldTransform_;
	//
	Object3d* worldTransformAlertRoad_;
	//モデル
	Model* model_ = nullptr;
	//危険な道用モデル
	Model* AlertRoadModel_ = nullptr;

	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	//弾の速度
	float speed = 1.5f;

	//速度
	Vector3 velocity_;
	//デスフラグ
	bool isDead_ = false;

	Vector3 worldSc = { 0.5,0.5,0.5 };

	bool hitRoad = false;

	//重力
	float gravity = 0.1f;

	float speedGra = 0;

	//床の座標
	float roadPos = -1.5f;

};