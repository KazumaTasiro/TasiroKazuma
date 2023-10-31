#pragma once
#include "Object3d.h"
#include "Model.h"
#include "Easing.h"
#include "SplinePosition.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "ParticleManager.h"

class  GameOverSeen
{
public:
	 GameOverSeen();
	~GameOverSeen();
	///<summary>
	////初期化
	///</summary>
	void Initialize();
	///<summary>
	////更新
	///</summary>
	void Update();
	///<summary>
	////描画
	///</summary>
	void Draw();
	///<summary>
	////パーティクル描画
	///</summary>
	void DrawParticle();

	///<summary>
	////カメラセット
	///</summary>
	void SetCamera(Camera* camera) {
		camera_ = camera;
	}
	void PlayerDeadParticle();

private:
	float PI = 3.141592f;

	Camera* camera_ = nullptr;
	Vector3 cameraPos_ = { 5,0,0 };

	Object3d* playerGameOver = nullptr;
	Model* playerGameOverModel = nullptr;
	Vector3 playerResetPos = { 0,-2.5f,-1.0f };
	Vector3 playerPos = { 0,-2.5f,0.0f };
	

	ParticleManager* playerDeadParticle;

	Vector3 playerRot = { ( PI / 180 ) * 90.0f,0,0 };

};