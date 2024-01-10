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
	float PI = 3.141592f;

	Camera* camera_ = nullptr;
	Vector3 cameraPos_ = { 5,0,0 };

	Object3d* playerGameOver = nullptr;
	Model* playerGameOverModel = nullptr;
	Vector3 playerResetPos = { 0,-2.5f,-1.0f };
	Vector3 playerPos = { 0,-2.5f,0.0f };
	

	ParticleManager* playerDeadParticle;

	Vector3 playerRot = { ( PI / 180 ) * 90.0f,0,0 };

	const float rnd_posS = 0.0f;
	//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
	const float rnd_velS = 0.005f;
	const float rnd_velYS = 0.05f;
	const float rnd_accS = 0.0000f;

	int particleLife = 150;
	float particleScaleStert = 0.0f;
	float particleScaleEnd = 1.0f;
};