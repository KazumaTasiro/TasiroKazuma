#pragma once
#include "GameScene.h"

class GameScene;

class GameSceneState
{
public:
	virtual ~GameSceneState() = default;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void showState() = 0;
	virtual void Finalize() = 0;

private:
	GameScene* scene_ = nullptr;
};
