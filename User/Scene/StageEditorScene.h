#pragma once
#include "GameSceneState.h"
#include "TitleScene.h"

#include "Road.h"
#include "Input.h"
#include "ParticleManager.h"
#include "Camera.h"
#include "RailCamera.h"
#include "StageEditor.h"
#include "ImGuiManager.h"

class StageEditorScene :public GameSceneState
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
	std::unique_ptr <Road> road_ = nullptr;
	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr <Skydome> skydome_ = nullptr;
	std::unique_ptr <Sprite> stert_ = nullptr;
	std::unique_ptr<RailCamera> railCamera = nullptr;

	ImGuiManager* ImGuiMan_ = nullptr;

	Vector3 cameraGame = { 0,0,-10 };
	Vector3 cameraRight = { 10,0,0 };
	Vector3 cameraLeft = { -10,0,0 };

	int CameraPos = 1;
};