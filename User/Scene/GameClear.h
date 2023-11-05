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
	//更新
	void Update();

private:
	float PI = 3.141592f;

	Camera* camera_ = nullptr;
	Vector3 cameraPos_ = { 5,0,0 };

	Object3d* playerGameOver = nullptr;
	Model* playerGameOverModel = nullptr;
	Vector3 playerResetPos = { 0,-2.5f,-1.0f };
	Vector3 playerPos = { 0,-2.5f,0.0f };


	ParticleManager* playerDeadParticle = nullptr;

	Vector3 playerRot = { ( PI / 180 ) * 90.0f,0,0 };
};
