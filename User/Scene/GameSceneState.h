#pragma once
#include "GameScene.h"

class SceneManager;

class GameSceneState
{
public:
	virtual ~GameSceneState() = default;
	virtual void Initialize() = 0;
	virtual void Update() = 0;
	virtual void ObjectDraw() = 0;
	virtual void SpriteDraw() = 0;
	virtual void Finalize() = 0;

	virtual void SetSceneManager(SceneManager* sceneManager) {
		sceneManager_ = sceneManager;
	}

private:
	SceneManager* sceneManager_ = nullptr;
};
