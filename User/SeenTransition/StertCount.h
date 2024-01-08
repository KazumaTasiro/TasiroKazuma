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
	bool GoStert() {
		return goStert;
	}
private:
	enum GoNmb
	{
		zero,
		one,
		two,
		three = 10,
	};

	enum Tex
	{
		oneSp = 15,
		twoSp = 14,
		threeSp = 13,
		goSp = 16,
	};
private://カウント
	Sprite* Time3 = nullptr;
	Sprite* Time2 = nullptr;
	Sprite* Time1 = nullptr;
	Sprite* TimeGo = nullptr;
private://カメラ
	Camera* camera_ = nullptr;

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
private://イージング
	Easing* easing = nullptr;
	int time = 0;
	int CengeSeenTIme = 5;
	int oneSecondTime = 120;
	int endTime = 210 * 2;
	int nowCount;
	float cameraPos;
	float cameraPos2;
	float cameraPos3;

	float cameraPosTime;

	int splineTime = 0;

	bool cameraStertEnd = false;
private://その他 

	SpriteCommon* spriteCommon_ = nullptr;

	Vector3 PlayerPos = { 0,-1.0f,0 };

	SplinePosition* spline = nullptr;

	bool goStert = false;
};