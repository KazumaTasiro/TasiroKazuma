#pragma once
#include "Object3d.h"
#include "Model.h"
#include "Easing.h"
#include "SplinePosition.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "ParticleManager.h"

class GameClear
{
public:
	GameClear();
	~GameClear();

	//初期化
	void Initialize();
	//描画
	void Draw();
	//描画
	void SpriteDraw();
	//更新
	void Update();
	void BossSpriteUpdate();
	///<summary>
	////パーティクル描画
	///</summary>
	void CameraUpdate();

	///<summary>
	////カメラセット
	///</summary>
	void SetCamera(Camera* camera) {
		camera_ = camera;
	}

	bool ClearTrue(){
		return clearEnd;
	}

	void Reset();
private:
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


	enum Tex
	{
		oneSp = 15,
		twoSp = 14,
		threeSp = 13,
		goSp = 16,
	};
	float PI = 3.141592f;

	SpriteCommon* spriteCommon_ = nullptr;
	Camera* camera_ = nullptr;
	Vector3 cameraPos_ = { 5,0,0 };

	Sprite* clear_ = nullptr;

	Object3d* bossSter = nullptr;
	Model* bossSterModel = nullptr;
	Vector3 bossPos = { 15,20,15 };
	Vector3 bossScale = { 10,10,10 };
	Vector3 bossRot = { ( PI / 180 )* -30,( PI / 180 ) * 200,0};

	float wCengePlus = 0.05f;
	float wCenge = 0.0f;

	float cameraPI = 0.07f;
	float cameraPIPuls = 0.0f;
	float cameraPIRs = 0.0f;
	float rotCamera = ( PI / 180 );
	Vector3 playerRot = { 0,cameraPIPuls * rotCamera,10 };

	float gameOverUp = 80.0f;

	bool cameraRotEnd = false;
	bool clearEnd = false;
};
