#pragma once
#include "GameSceneState.h"


class SceneManager
{
public:
		//シングルトン
	static SceneManager* GetInstance();

	//次シーン予約
	void SetNextScene(GameSceneState* nextScene){
		nextScene_ = nextScene;
	}
	void Initialize();

	void Update();

	void ObjectDraw();

	void SpriteDraw();

private:
	SceneManager() = default;
	~SceneManager();
	SceneManager(const SceneManager&) = delete;
	const SceneManager& operator=(const SceneManager&) = delete;

	static SceneManager* sceneManager;
private:
	//現在のシーン
	GameSceneState* scene_ = nullptr;
	//次のシーン
	GameSceneState* nextScene_ = nullptr;
};
