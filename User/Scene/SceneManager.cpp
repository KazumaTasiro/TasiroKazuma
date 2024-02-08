#include "SceneManager.h"

SceneManager* SceneManager::sceneManager = nullptr;

SceneManager* SceneManager::GetInstance()
{
	if ( sceneManager == nullptr )
	{
		sceneManager = new SceneManager();
	}
	return sceneManager;
}

void SceneManager::Initialize()
{
}

void SceneManager::Update()
{
	//次シーンの予約があるなら
	if ( nextScene_ )
	{
		//旧シーンの終了
		if ( scene_ )
		{
			scene_->Finalize();
			delete scene_;
		}

		//シーンの切り替え
		scene_ = nextScene_;
		nextScene_ = nullptr;
		//次シーンを初期化する
		scene_->Initialize();
	}
	//実行中のシーンの更新
	scene_->Update();
}

void SceneManager::Draw()
{
	scene_->Draw();
}

SceneManager::~SceneManager()
{
	scene_->Finalize();
	delete scene_;
}
