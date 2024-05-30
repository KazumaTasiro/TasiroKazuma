#include "GameOverScene.h"
#include "SceneManager.h"

void GameOverScene::Initialize()
{
	camera_ = std::make_unique<Camera>(WinApp::window_width,WinApp::window_height);
	camera_->SetEye(cameraGame);
	camera_->Update();


	lightGroupNon = LightGroup::Create();

	Object3d::SetLightNon(lightGroupNon);
	Object3d::SetCamera(camera_.get());
	ParticleManager::SetCamera(camera_.get());

	particleManager_ = std::make_unique <ParticleManager>();
	particleManager_->Initialize();
	particleManager_->LoadTexture("Explosion.png");

	skydome_ = std::make_unique <Skydome>();
	skydome_->Initalize();

	road_ = std::make_unique <Road>();
	road_->Initialize();

	stert_ = std::make_unique <Sprite>();
	stert_->Initialize(3);
	stert_->SetPozition({ WinApp::GetInstance()->window_width / 2.0f, WinApp::GetInstance()->window_height / 2.0f + gamestertUp });
	gameOver_ = std::make_unique <Sprite>();
	gameOver_->Initialize(6);
	gameOver_->SetPozition({ WinApp::GetInstance()->window_width / 2.0f,( WinApp::GetInstance()->window_height / 2.0f ) - gameOverUp});

	gameOver = std::make_unique<GameOver>();
	gameOver->Initialize();
	gameOver->SetCamera(camera_.get());
}

void GameOverScene::Update()
{
	skydome_->Update();
	road_->roadUpdate();
	gameOver->Update();
	SeenTransition::GetInstance()->Update();
	camera_->SetTarget(TargetCamRes);
	if ( SeenTransition::GetInstance()->ReturnSeenNotEnd() == false )
	{
		if (Input::GetInstance()->TriggerMouse(0) )
		{
			SeenTransition::GetInstance()->OnSeenTrans();
			seenFlag = true;
		}
	}
	if ( seenFlag )
	{
		if ( SeenTransition::GetInstance()->ReturnSeenTrans() )
		{
			GameSceneState* state = new TitleScene();
			SceneManager::GetInstance()->SetNextScene(state);
			seenFlag = false;
		}
	}

	LightData::GetInstance()->Update();
}

void GameOverScene::ObjectDraw()
{
	
	skydome_->Draw();
	road_->Draw();
	gameOver->Draw();
	gameOver->DrawParticle();
}

void GameOverScene::SpriteDraw()
{

	gameOver_->Draw();
	stert_->Draw();
	SeenTransition::GetInstance()->Draw();
}



void GameOverScene::Finalize()
{
}
