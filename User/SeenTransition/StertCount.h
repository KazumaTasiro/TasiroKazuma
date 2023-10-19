#pragma once
#include "Object3d.h"
#include "Model.h"
#include "Easing.h"


class StertCount
{
public:
	StertCount();
	~StertCount();
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
	////カメラのイージング
	///</summary>
	void CameraEasing();
	///<summary>
	////スタート画面リセット
	///</summary>
	void Reset();

private:
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
	int endTime = 120;
};