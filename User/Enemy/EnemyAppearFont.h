#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>

///<summary>
///ボス登場時の文字
///</summary>
class EnemyAppearFont
{
public:
	///<summary>
	///初期化
	///</summary>

	void Initialize(Object3d* boss);

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
	bool IsDead()const {
		return isDead_;
	}

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	///<summary>
	////文字の座標
	///</summary>
	Vector3 GetWorldPosition(int nmb);

	///<summary>
	////文字のサイズを変える
	///</summary>
	void SetSize(Vector3 Size,int nmb) {
		fontWorldTransform_[nmb]->wtf.scale = Size;
	}
	///<summary>
	////文字の回転攻撃
	///</summary>
	void Attck();

	void OnAttckFlag() {
		nowAttck = true;
	}

private:
	//ワールド変換データ
	Object3d* fontWorldTransform_[4];
	Vector3 fontSize = { 10,10,10 };
	//モデル
	Model* fontModel_[ 4 ] = { nullptr };
	//速度
	Vector3 velocity_;

	Vector3 fontPos4th = { 50.0f,50.0f,0 };

	//寿命<fim>
	static const int32_t kLifeTime = 60 * 1000;

	//デスタイマー
	int32_t deathTimer_ = kLifeTime;

	//文字を飛ばす間隔
	float attackInterval = 60;
	//飛ぶ文字の番号Zの順番に飛ばす
	int32_t fontAttckNmb = 0;

	//デスフラグ
	bool isDead_ = false;
	//攻撃中フラグ
	bool nowAttck = false;

	int fontI = 4;
};
