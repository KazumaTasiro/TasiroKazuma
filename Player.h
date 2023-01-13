#pragma once
#include"Object3d.h"
#include "Model.h"
#include <cassert>
#include "Input/Input.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include <WinApp.h>
#include "Sprite.h"

///<summary>
///自キャラ
///</summary>
class Player {
public:
	Player();
	~Player();
	///<summary>
	///初期化
	///</summary>
	void Initialize(SpriteCommon* spriteCommon,Input* input);
	///更新
	///</summary>
	void Update();
	void Move();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	XMFLOAT3 ConvertToXMFLOAT3(const XMMATRIX &mat, XMFLOAT3 vec);

	XMFLOAT3 GetWorldPosition();

	XMFLOAT3 XMFLOAT3Normalize(XMFLOAT3& v);


	///<summary>
	///描画
	///</summary>
	void Draw();
	///<summary>
	///攻撃
	///</summary>
	void Attack();

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//ベクトルを正規化する
	int Vec3Normalize(XMFLOAT3* pOut, XMFLOAT3* pV);

	///<summary>
	///UI描画
	///</summary>
	void DrawUI();

	XMFLOAT3 clossV3V4(const XMFLOAT3& vec, const XMMATRIX& mat);

	/*int MatrixInverse(XMMATRIX& pOut, XMMATRIX& pM);*/
	XMFLOAT3 AddVector(const XMFLOAT3 v1, const XMFLOAT3 v2);

	void Reset();

private:
	//ワールド変換データ
	Object3d* worldTransform_;

	//3Dレティクル用ワールドトランスフォーム
	Object3d* worldTransform3DReticle_;

	//モデル
	Model* model_ = nullptr;
	//弾モデル
	Model* bulletModel_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_;
	SpriteCommon* spriteCommon_ = nullptr;
	//デスフラグ
	bool isDead_ = false;

};