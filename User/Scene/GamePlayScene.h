#pragma once
#include "GameSceneState.h"
#include "GameClearScene.h"
#include "GameOverScene.h"

#include "Road.h"
#include "Input.h"
#include "Player.h"
#include "ParticleManager.h"
#include "Camera.h"
#include "RailCamera.h"
#include "EnemyManager.h"
#include "StertCount.h"
#include "RailCamera.h"

class GamePlayScene:public GameSceneState
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
	std::unique_ptr<SeenTransition> seenTransition_ = nullptr;
	std::unique_ptr<Player> player_ = nullptr;
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;
	std::unique_ptr<ParticleManager> particleManager_ = nullptr;
	std::unique_ptr<Camera> camera_ = nullptr;
	std::unique_ptr<RailCamera> railCamera = nullptr;
	std::unique_ptr <Skydome> skydome_ = nullptr;
	std::unique_ptr <StertCount> stertCount_ = nullptr;
	std::unique_ptr <Sprite> operation_ = nullptr;
	std::unique_ptr <Sprite> cameraMoveOps_ = nullptr;
	std::unique_ptr <Sprite> moveTute_ = nullptr;


	float bossTime = 3.0f;
	Vector3 cameraGame = { 0,0,-10 };
	Vector2 operationPos = { 128,64 };
};
