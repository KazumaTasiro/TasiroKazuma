#include "GameClearScene.h"
#include "SceneManager.h"

void GameClearScene::Initialize()
{
	camera_ = std::make_unique<Camera>(WinApp::window_width,WinApp::window_height);
	camera_->SetEye(cameraGame);
	camera_->Update();
	Object3d::SetCamera(camera_.get());
	ParticleManager::SetCamera(camera_.get());

	particleManager_ = std::make_unique <ParticleManager>();
	particleManager_->Initialize();
	particleManager_->LoadTexture("Explosion.png");

	skydome_ = std::make_unique <Skydome>();
	skydome_->Initalize();

	road_= std::make_unique <Road>();
	road_->Initialize();

	stert_ = std::make_unique <Sprite>();
	stert_->Initialize(3);
	stert_->SetPozition({ WinApp::GetInstance()->window_width / 2.0f, WinApp::GetInstance()->window_height / 2.0f + gamestertUp});

	player_ = std::make_unique<Player>();
	player_->Initialize(particleManager_.get());
	player_->SetCamera(camera_.get());
}

void GameClearScene::Update()
{
	gameClearScene->Update();
	road_->roadUpdate();
	seenTransition_->Update();
	player_->ClearMove();
	if ( seenTransition_->ReturnSeenNotEnd() == false )
	{
		if ( gameClearScene->ClearTrue() )
		{
			if ( Input::GetInstance()->TriggerMouse(0) )
			{
				seenTransition_->OnSeenTrans();
				seenFlag = true;
			}
		}
	}
	if ( seenFlag )
	{
		if ( seenTransition_->ReturnSeenTrans() )
		{
			GameSceneState* state = new TitleScene();
			SceneManager::GetInstance()->SetNextScene(state);
			seenFlag = false;
		}
	}
	LightData::GetInstance()->Update();
}

void GameClearScene::ObjectDraw()
{
	skydome_->Draw();
	road_->Draw();
	gameClearScene->Draw();
	player_->Draw();
}

void GameClearScene::SpriteDraw()
{
			//gameClear_->Draw();
	gameClearScene->SpriteDraw();
	if ( gameClearScene->ClearTrue() )
	{
		stert_->Draw();
	}
}

void GameClearScene::Finalize()
{
}
