#pragma once

#include "GameSceneState.h"
#include "TitleScene.h"

#include "Road.h"
#include "Input.h"
#include "ParticleManager.h"
#include "LightGroup.h"
#include "Camera.h"

class GameOverScene :public GameSceneState
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
	std::unique_ptr <GameOver> gameOver = nullptr;
	std::unique_ptr <Road> road_ = nullptr;
	std::unique_ptr<ParticleManager> particleManager_ = nullptr;
	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr <Skydome> skydome_ = nullptr;
	std::unique_ptr <Sprite> stert_ = nullptr;
	std::unique_ptr <Sprite> gameOver_ = nullptr;
	LightGroup* lightGroupNon = nullptr;


	Vector3 TargetCamRes = { 0,0,0 };
	float gameOverUp = 80.0f;
	float gamestertUp = 50.0f;
	Vector3 cameraGame = { 0,0,-10 };
	bool seenFlag = false;
};