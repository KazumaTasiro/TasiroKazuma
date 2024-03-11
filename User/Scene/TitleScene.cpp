#include "TitleScene.h"
#include "SceneManager.h"

void TitleScene::Initialize()
{
	camera_ = std::make_unique<Camera>(WinApp::window_width,WinApp::window_height);
	camera_->SetEye(cameraTitle);
	camera_->Update();
	Object3d::SetCamera(camera_.get());
	ParticleManager::SetCamera(camera_.get());
	skydome_ = std::make_unique <Skydome>();
	skydome_->Initalize();

	road_ = std::make_unique <Road>();
	road_->Initialize();

	stert_ = std::make_unique <Sprite>();
	stert_->Initialize(3);
	stert_->SetPozition({ WinApp::GetInstance()->window_width / 2.0f, WinApp::GetInstance()->window_height / 2.0f + gamestertUp });

	title_ = std::make_unique< Titles>();
	title_->Initialize();
}

void TitleScene::Update()
{


	road_->BeforeUpdate();
	title_->Update();
	seenTransition_->Update();
	//player_->Update();
	if ( seenTransition_->ReturnSeenNotEnd() == false )
	{
		if ( Input::GetInstance()->TriggerMouse(0) )
		{
			seenTransition_->OnSeenTrans();
			seenFlag = true;
		}
	}
	if ( seenFlag )
	{
		if ( seenTransition_->ReturnSeenTrans() )
		{

			
			GameSceneState* state = new GamePlayScene();
			SceneManager::GetInstance()->SetNextScene(state);
			seenFlag = false;
		}
	}
}

void TitleScene::ObjectDraw()
{
	skydome_->Draw();
	road_->Draw();
	/*player_->Draw();*/
	title_->Draw();
}

void TitleScene::SpriteDraw()
{
	stert_->Draw();
}

void TitleScene::Finalize()
{
}
