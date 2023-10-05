#pragma once
#include"Object3d.h"
#include "Model.h"
#include <cassert>
#include "Input.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include <WinApp.h>
#include "Sprite.h"
#include "Vector3.h"
#include "ConvertXM.h"
#include "Affin.h"
#include "ImGuiManager.h"
#include "ParticleManager.h"
#include "FbxLoader.h"
#include "Object3dFbx.h"
#include "DirectXCommon.h"

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
	void Initialize(SpriteCommon* spriteCommon, Input* input, WinApp* winApp_, DirectXCommon* dxCommon, ParticleManager* particle);
	///更新
	///</summary>
	void Update();
	void Move();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	Vector3 ConvertToVector3(const Matrix4& mat, Vector3 vec);

	Vector3 GetWorldPosition();


	///<summary>
	///描画
	///</summary>
	void Draw();

	void DrawFbx();
	///<summary>
	///攻撃
	///</summary>
	void Attack();

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//ベクトルを正規化する
	int Vec3Normalize(Vector3* pOut, Vector3* pV);

	///<summary>
	///UI描画
	///</summary>
	void DrawUI();

	Vector3 clossV3V4(const Vector3& vec, const XMMATRIX& mat);

	/*int MatrixInverse(XMMATRIX& pOut, XMMATRIX& pM);*/
	Vector3 AddVector(const Vector3 v1, const Vector3 v2);

	void MouseReticle();

	void Reset();

	Vector2 GetReticlePos();

	Vector3 GetFarNear();

	void ReticleLimit();

	void PlayerLimit();

	bool retrunIsDaed() { return isDead_; }

	void SetPos(Vector3 Pos) { worldTransform_->wtf.position = Pos; }

	void PlayerDeadParticle();

	void ParticleDraw();

	void ClearMove();

	void TitleMove();
private:
	//ワールド変換データ
	Object3dFbx* worldTransform_;

	//3Dレティクル用ワールドトランスフォーム
	Object3d* worldTransform3DReticle_;

	//Object3dFbx* fbxWorldTransform_ = nullptr;
	//FbxModel* fbxModel_ = nullptr;

	//モデル
	FbxModel* model_ = nullptr;
	//弾モデル
	Model* bulletModel_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_;
	Sprite* sprite2DReticleLock_;
	SpriteCommon* spriteCommon_ = nullptr;
	//デスフラグ
	bool isDead_ = false;
	WinApp* winApp = nullptr;
	DirectXCommon* dxCommon_ = nullptr;

	Vector3 farCre;
	Vector3 nearCre;

	Vector3 moveLimit = { 20,20,20 };
	bool limitIn = false;

	Vector3 playerResetPos = { 0,-2.5f,-30 };

	const float speed = 0.5f;

	int playerHp;

	bool DeadParticle = false;

	int EffectWaiteTime = 50;

	ParticleManager* playerDeadParticle;
};