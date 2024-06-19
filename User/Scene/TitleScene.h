#pragma once

#include "GameSceneState.h"
#include "GamePlayScene.h"
#include "StageEditorScene.h"

#include "Road.h"
#include "LightGroup.h"
#include "Input.h"
#include "Camera.h"
#include "Title.h"
#include "ImGuiManager.h"
#include "Input.h"

class TitleScene :public GameSceneState
{
public:
	//初期化
	void Initialize() override;
	//更新
	void Update() override;
	//描画
	void ObjectDraw() override;
	void SpriteDraw() override;
	//終了処理
	void Finalize() override;


private:
	std::unique_ptr <Titles> title_ = nullptr;
	std::unique_ptr <Road> road_ = nullptr;
	
	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr <Skydome> skydome_ = nullptr;
	std::unique_ptr <Sprite> stert_ = nullptr;

	ImGuiManager* ImGuiMan_ = nullptr;

	LightGroup* lightGroupNon = nullptr;

	Vector3 cameraImgui = { 0,0,-10.0f };

	Vector3 TargetCamRes = { 0,0,0 };
	float gameOverUp = 80.0f;
	float gamestertUp = 50.0f;
	Vector3 cameraTitle = { 0,0,10 };
	bool seenFlag = false;
};