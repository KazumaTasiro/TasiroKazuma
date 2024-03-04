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
#include "Camera.h"
#include "LightData.h"
#include "ParticleLibrary.h"


///<summary>
///自キャラの行動
///</summary>
class Player
{
public:
	Player();
	~Player();
	///<summary>
	///初期化
	///</summary>
	void Initialize(ParticleManager* particle);
	///更新
	///</summary>
	void Update();
	void AttackUpdate();
	void ReticleUpdate();
	void cameraUpdate();

	void Move();

	//衝突を検出したら呼び出されるコールバック関数
	void OnCollision();
	///<summary>
	////Matrix$をVector3に
	///</summary>
	Vector3 ConvertToVector3(const Matrix4& mat,Vector3 vec);
	///<summary>
	////プレイヤーの座標
	///</summary>
	Vector3 GetWorldPosition();


	///<summary>
	///描画
	///</summary>
	void Draw();
	///<summary>
	////FBXの描画
	///</summary>
	void DrawFbx();
	///<summary>
	///攻撃
	///</summary>
	void Attack();

	//弾リストを取得
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() {
		return bullets_;
	}


	///<summary>
	///UI描画
	///</summary>
	void DrawUI();
	///<summary>
	////外積
	///</summary>
	Vector3 clossV3V4(const Vector3& vec,const XMMATRIX& mat);

	/*int MatrixInverse(XMMATRIX& pOut, XMMATRIX& pM);*/
	///<summary>
	////Vector3を足すやつ
	///</summary>
	Vector3 AddVector(const Vector3 v1,const Vector3 v2);
	///<summary>
	////マウスをレティクル
	///</summary>
	void MouseReticle();
	///<summary>
	////リセット
	///</summary>
	void Reset();
	///<summary>
	////レティクルの座標
	///</summary>
	Vector2 GetReticlePos();
	///<summary>
	////ファークリップとニアクリップのベクトル
	///</summary>
	Vector3 GetFarNear();
	///<summary>
	////レティクルの行動限界
	///</summary>
	void ReticleLimit();
	///<summary>
	////プレイヤーの行動限界
	///</summary>
	void PlayerLimit();
	///<summary>
	////プレイヤーの死亡判定
	///</summary>
	bool retrunIsDaed() {
		return isDead_;
	}
///<summary>
////座標更新
///</summary>
	void SetPos(Vector3 Pos) {
		worldTransform_->wtf.position = Pos;
	}

///<summary>
////死亡演出
///</summary>
	void PlayerDeadParticle();
	///<summary>
	////演出描画
	///</summary>
	void ParticleDraw();
	///<summary>
	////クリア演出
	///</summary>
	void ClearMove();
	///<summary>
	////タイトル時の行動
	///</summary>
	void TitleMove();

	//プレイヤーのHPバー更新
	void PlayerHpUpdate();

	void SetCamera(Camera* camera) {
		camera_ = camera;
	}

	void DamageShakeUpdate();

	void ResetDamageFlag();

	void ImguiDraw();
private:

	float PI = 3.141592f;

	enum Nmb
	{
		zero = 0,
		one = 1,
		two = 2,
		three = 3,
		four = 4,
		five = 5,
		six = 6,
		seven = 7,
		eight = 8,
		nine = 9,
		ten = 10,
	};
	const float zeroNmb = 0.0f;
	const float oneNmb = 1.0f;
	const float twoNmb = 2.0f;
	const float threeNmb = 3.0f;
	const float fourNmb = 4.0f;
	const float fiveNmb = 5.0f;
	const float sixNmb = 6.0f;
	const float sevenNmb = 7.0f;
	const float eightNmb = 8.0f;
	const float nineNmb = 9.0f;
	const float tenNmb = 3.0f;

	const int zeroNmbI = 0;
	const int oneNmbI = 1;
	const int twoNmbI = 2;
	const int threeNmbI = 3;
	const int fourNmbI = 4;
	const int fiveNmbI = 5;
	const int sixNmbI = 6;
	const int sevenNmbI = 7;
	const int eightNmbI = 8;
	const int nineNmbI = 9;
	const int tenNmbI = 10;


	//3Dレティクル用ワールドトランスフォーム
	Object3d* worldTransform3DReticle_;

		//ワールド変換データ
	Object3d* worldTransform_;

	//Object3dFbx* fbxWorldTransform_ = nullptr;
	//FbxModel* fbxModel_ = nullptr;

	//モデル
	Model* model_ = nullptr;
	//弾モデル
	Model* bulletModel_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;

	Camera* camera_ = nullptr;

	//弾
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	//2Dレティクル用スプライト
	Sprite* sprite2DReticle_;
	Sprite* sprite2DReticleLock_;
	SpriteCommon* spriteCommon_ = nullptr;

	Sprite* playerHPNone;
	Sprite* playerHPMax;

	float hpSpriteHight = 124.0f;
	Vector2 spriteHpSize = {};
	Vector2 spritePos = { 16.0f,156.0f };

	//デスフラグ
	bool isDead_ = false;
	WinApp* winApp = nullptr;

	Vector3 farCre;
	Vector3 nearCre;

	Vector3 moveLimit = { 20,20,20 };
	bool limitIn = false;

	Vector3 playerResetPos = { 0,-2.5f,0 };

	 float speed = 0.1f;
	float speedPlus = 0.01f;
	float speedMax = 0.15f;

	float playerHp;
	float playerMaxHp = 10.0f;

	bool DeadParticle = false;

	int EffectWaiteTime = 50;

	ParticleManager* playerDeadParticle;

	Vector3 playerSc = { 1.0f, 1.0f, 1.0f };
	Vector3 playerPos = { 0, 0, -20 };

	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 19.0f;

	const float kBulletSpeed = 0.01f;
	const float kDistanceTestObject = 120.0f;

	int particleLife = 30;
	float particleScaleStert = 0.0f;
	float particleScaleEnd = 25.0f;
	Vector3 clearMove = { 0,0,1 };

	float posLim = 4;
	float RetiRim = 0;

	const float rnd_posS = 5.0f;
	const float rnd_velS = 0.0f;
	const float rnd_accS = 0.0000f;

	float damageShakeX = 0.0f;
	float damageShakeY = 0.0f;
	float damageShakeZ = 0.0f;

	Vector3 damageShakeBefor;


	float shakeLimit = 0.5f;

	bool shakeFlag = false;

	Sprite* damageEffect = nullptr;

	int shakeTime = 10;
	int shakeTimeRe = 10;

	int playerAttckTime = 20;
	int playerAttckTimeRe = 20;

	uint32_t shadowNmb = 0;

	Vector4  circleShadowDir = { 0,0.5f,0,0 };
	Vector3  circleShadowAtten = { 1.3f,2.0f,0.0f };
	Vector2  circleShadowFactorAngle = { 0.25f,1.14f };

	bool lightActive = true;

	float playerRot = 0;
	float playerRotReset = 0;
	float playerRotLimit = 10.0f;
	float playerRotPlus = 1.0f;
	float rot = ( PI / 180 );
};