#pragma once
#include "Object3d.h"
#include "Model.h"
#include "Easing.h"
#include "SplinePosition.h"
#include "SpriteCommon.h"
#include "Sprite.h"

class StertCount
{
public:
	StertCount();
	~StertCount();
	///<summary>
	////初期化
	///</summary>
	void Initialize(SpriteCommon* spriteCommon);
	///<summary>
	////更新
	///</summary>
	void Update();
	///<summary>
	////描画
	///</summary>
	void Draw();
	///<summary>
	////カメラセット
	///</summary>
	void SetCamera(Camera* camera) {
		camera_ = camera;
	}
	///<summary>
	////カメラアップデート
	///</summary>
	void CameraUpdate();
	///<summary>
	////ボスアップデート
	///</summary>
	void BossUpdate();
	///<summary>
	////カメラのイージング(実装見送り)
	///</summary>
	void CameraEasing();
	///<summary>
	////カメラの移動
	///</summary>
	void CameraMove();
	///<summary>
	////スタート画面リセット
	///</summary>
	void Reset();
	///<summary>
	////スタート画面終わり
	///</summary>
	bool stertEnd() {
		return cameraStertEnd;
	}
private:
	Sprite* Time3 = nullptr;
	Sprite* Time2 = nullptr;
	Sprite* Time1 = nullptr;
	Sprite* TimeGo = nullptr;
	SpriteCommon* spriteCommon_ = nullptr;
	Camera* camera_ = nullptr;
	Object3d* bossStert = nullptr;
	Model* bossStertModel = nullptr;
	//ボススケール
	Vector3 BossScale = { 20,20,20 };
	//ボスポジション
	Vector3 StertBossPos = { 0,10, -500 };
	float stertBossFrontCamera = 90.0f;

	Easing* easing = nullptr;
	int time = 0;
	int CengeSeenTIme = 20;
	int oneSecondTime = 120;
	int endTime = 240*2;
	int nowCount;
	float cameraPos;
	float cameraPos2;
	float cameraPos3;

	float cameraPosTime;

	int splineTime = 0;

	bool cameraStertEnd = false;

	Vector3 PlayerPos = { 0,-1.0f,0 };

	Vector3 RightCameraPos = { 3,-1,-5 };
	Vector3 RightCameraAfterPos = { 3,-1,20 };

	Vector3 LeftCameraPos = { -3,-1,-5 };
	Vector3 LeftCameraAfterPos = { -3,-1,20 };

	Vector3 CenterCameraPos = { 0,0,-5 };
	Vector3 CenterCameraAfterPos = { 0,0,-15 };

	Vector3 TargetReset = { 0,0,0 };

	Vector3 CameraMoveSpline0 = { 0,0,0 };
	Vector3 CameraMoveSpline1 = { 3,-2.5f,-10 };
	Vector3 CameraMoveSpline2 = { -20,30,100 };

	SplinePosition* spline = nullptr;
};