#include "GamePlayScene.h"
#include "SceneManager.h"

void GamePlayScene::Initialize()
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


	railCamera = std::make_unique < RailCamera>();
	railCamera->SetCamera(camera_.get());
	railCamera->SetPlayer(player_.get());
	railCamera->Initialize();
	railCamera->Update();

	player_ = std::make_unique<Player>();
	player_->Initialize(particleManager_.get());
	player_->SetCamera(camera_.get());

	enemyManager_ = std::make_unique < EnemyManager>();
	enemyManager_->Initialize(camera_.get(),particleManager_.get());
	enemyManager_->SetPlayer(player_.get());

	stertCount_ = std::make_unique<StertCount>();
	stertCount_->SetCamera(camera_.get());
	stertCount_->Initialize();


	operation_ = std::make_unique <Sprite>();
	operation_->Initialize(19);
	operation_->SetPozition({ WinApp::GetInstance()->window_width- operationPos.x,( WinApp::GetInstance()->window_height  ) - operationPos.y });

	cameraMoveOps_ = std::make_unique <Sprite>();
	cameraMoveOps_->Initialize(20);
	cameraMoveOps_->SetPozition({ WinApp::GetInstance()->window_width - operationPos.x,( WinApp::GetInstance()->window_height ) - ( operationPos.y * 3 ) });

	moveTute_ = std::make_unique <Sprite>();
	moveTute_->Initialize(24);
	moveTute_->SetPozition({ WinApp::GetInstance()->window_width  - operationPos.x,( WinApp::GetInstance()->window_height ) - ( operationPos.y * 5 ) });
}

void GamePlayScene::Update()
{
#ifdef NDEBUG
	CursorLimit();

#endif // DEBUG

	skydome_->Update();
	if ( enemyManager_->Clear() == false ){ 
	road_->Update();
	stertCount_->Update();
	if ( stertCount_->GoStert() )
	{
		player_->cameraUpdate();
	}
	SeenTransition::GetInstance()->Update();
	camera_->Update();
	player_->Update();

	if ( stertCount_->stertEnd() )
	{
		if ( player_->retrunIsDaed() == false )
		{
			railCamera->Update();
		}
		//player_->Update();
		player_->AttackUpdate();
		enemyManager_->SetPlayer(player_.get());
		enemyManager_->Update();
		enemyManager_->EnemyCollision(player_.get());

		if ( player_->retrunIsDaed() )
		{
			SeenTransition::GetInstance()->OnSeenTrans();
			if ( SeenTransition::GetInstance()->ReturnSeenTrans() )
			{
				GameSceneState* state = new GameOverScene();
				SceneManager::GetInstance()->SetNextScene(state);
			}
		}

	}

	}
	if ( enemyManager_->Clear() == true )
	{
		bossTime--;
		SeenTransition::GetInstance()->Update();
		player_->cameraUpdate();
		road_->Update();
		enemyManager_->SetPlayer(player_.get());
		enemyManager_->BossUpdate();
		enemyManager_->Update();
		enemyManager_->EnemyCollision(player_.get());
		camera_->Update();
		player_->Update();
		player_->AttackUpdate();
		if ( enemyManager_->BossClear() )
		{
			player_->ResetDamageFlag();
			SeenTransition::GetInstance()->OnSeenTrans();
			if ( SeenTransition::GetInstance()->ReturnSeenTrans() )
			{
				GameSceneState* state = new GameClearScene();
				SceneManager::GetInstance()->SetNextScene(state);
			}

		}
		if ( player_->retrunIsDaed() )
		{
			SeenTransition::GetInstance()->OnSeenTrans();
			if ( SeenTransition::GetInstance()->ReturnSeenTrans() )
			{
				GameSceneState* state = new GameOverScene();
				SceneManager::GetInstance()->SetNextScene(state);

			}
		}
	}
		
	LightData::GetInstance()->Update();
}

void GamePlayScene::ObjectDraw()
{
	skydome_->Draw();
	road_->Draw();
	player_->Draw();

	enemyManager_->Draw();
	if ( bossTime < 0 )
	{
		enemyManager_->BossDraw();
	}
	enemyManager_->ParticleDraw();
	player_->ParticleDraw();
}

void GamePlayScene::SpriteDraw()
{
	SeenTransition::GetInstance()->Draw();
	if ( stertCount_->stertEnd() )
	{
		player_->DrawUI();
		operation_->Draw();
		cameraMoveOps_->Draw();
		moveTute_->Draw();
	}
	stertCount_->Draw();
	enemyManager_->DrawUI();
}


void GamePlayScene::Finalize()
{

}

void GamePlayScene::CursorLimit()
{
	

	GetClipCursor(&rcOldClip);
	GetWindowRect(WinApp::GetInstance()->GetHwnd(),&rcClip);
	ClipCursor(&rcClip);
	ClipCursor(&rcOldClip);
}
