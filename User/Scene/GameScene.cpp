#include "GameScene.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{

}
void GameScene::Initialize(DirectXCommon* dxcomon)
{
	assert(dxcomon);


	input_ = Input::GetInstance();

	winApp_ = WinApp::GetInstance();

	dxCommon_ = dxcomon;


	audio_ = std::make_unique <Audio>();
	audio_->Initialize();

	// カメラ生成
	camera_ = std::make_unique <Camera>(WinApp::window_width,WinApp::window_height);
	camera_->SetEye(cameraTitle);

	camera_->Update();
	Object3d::SetCamera(camera_.get());
	ParticleManager::SetCamera(camera_.get());

	ImGuiMan_ = ImGuiManager::GetInstance();

	//スプライト共通部分の初期化
	




	skydome_ = std::make_unique < Skydome>();
	skydome_->Initalize();

	stert_ = std::make_unique <Sprite>();
	stert_->Initialize(three);
	stert_->SetPozition({ winApp_->window_width / 2,winApp_->window_height / 2 + gamestertUp });


	gameClear_ = std::make_unique <Sprite>();
	gameClear_->Initialize(five);
	gameClear_->SetPozition({ winApp_->window_width / 2,( winApp_->window_height / 2 ) - gameOverUp });
	spriteEnd_ = { winApp_->window_width / 2,winApp_->window_height / 2 };
	gameOver_ = std::make_unique <Sprite>();
	gameOver_->Initialize(six);
	gameOver_->SetPozition({ winApp_->window_width / 2,( winApp_->window_height / 2 ) - gameOverUp });
	operation_ = std::make_unique <Sprite>();
	operation_->Initialize(19);
	operation_->SetPozition({ WinApp::window_width - operationPos.x,WinApp::window_height - operationPos.y });
	//operation_->SetAnchorPoint({ 0,0 })
	cameraMoveOps_= std::make_unique <Sprite>();
	cameraMoveOps_->Initialize(20);
	cameraMoveOps_->SetPozition({ WinApp::window_width - operationPos.x,WinApp::window_height - (operationPos.y*3) });
	moveTute_ = std::make_unique <Sprite>();
	moveTute_->Initialize(24);
	moveTute_->SetPozition({ WinApp::window_width - operationPos.x,WinApp::window_height - ( operationPos.y * 5 ) });

	road_ = std::make_unique <Road>();
	road_->Initialize();


	Object3dFbx::SetDevice(dxCommon_->GetDevice());
	Object3dFbx::SetCamera(camera_.get());
	Object3dFbx::CreateGraphicsPipeline();

	ParticleMana_ = std::make_unique <ParticleManager>();
	ParticleMana_->Initialize();
	ParticleMana_->LoadTexture("Explosion.png");

	player_ = std::make_unique<Player>();
	player_->Initialize(ParticleMana_.get());
	player_->SetCamera(camera_.get());

	enemyManager_ = std::make_unique < EnemyManager>();
	enemyManager_->Initialize(camera_.get(),ParticleMana_.get());

	enemyManager_->SetGameScene(this);
	enemyManager_->SetPlayer(player_.get());

	//player_->Update();
	seenTransition_ = std::make_unique < SeenTransition>();
	seenTransition_->Initialize();

	gameOverSeen = std::make_unique<GameOver>();
	gameOverSeen->Initialize();
	gameOverSeen->SetCamera(camera_.get());

	gameClearScene = std::make_unique < GameClear>();
	gameClearScene->SetCamera(camera_.get());
	gameClearScene->Initialize();

	title_ = std::make_unique< Titles>();
	title_->Initialize();


	scene = Scene::Title;
	stertCount_ = std::make_unique<StertCount>();
	stertCount_->SetCamera(camera_.get());
	railCamera = std::make_unique < RailCamera>();
	railCamera->SetCamera(camera_.get());
	railCamera->SetPlayer(player_.get());
	railCamera->Initialize();
	railCamera->Update();

	lightGroupNon = LightGroup::Create();

	//direLight = DirectionalLight::Create();

	//lightGroupUse->SetDirLightActive(0,true);
	//lightGroupUse->SetPointLightActive(0,true);
	//lightGroupUse->SetCircleShadowActive(0,true);



	Object3d::SetLightNon(lightGroupNon);
	ParticleLibrary::GetInstance()->ObjectInitialize();

	ParticleLibrary::GetInstance()->ParticleDataSave(0,"RotParticleL");
	ParticleLibrary::GetInstance()->ParticleUpdate(0);
	ParticleLibrary::GetInstance()->ParticleDataSave(1,"RotParticleR");
	ParticleLibrary::GetInstance()->ParticleUpdate(1);
	StageEditor::GetInstance()->Initialize();
	PhaseReset();

}
void GameScene::Update()
{

	//direLight->Update();
	ImGuiMan_->Bigin();
#ifdef _DEBUG
	if ( input_->PushKey(DIK_P) )
	{
		scene = Editor;
	}
	switch ( scene )
	{
	case GameScene::Editor:
		StageEditor::GetInstance()->Update();
		StageEditor::GetInstance()->DrawImgui();

		break;
	}
#endif
#ifdef _PARTICLELIBRARY

		ImGui::SliderFloat3("cameraEye",&cameraImgui.x,-30,30);

		camera_->SetEye(cameraImgui);
		Vector3 CameraMove = {0,0,1.0f };
		Vector3 cameraTage = cameraImgui + CameraMove;
		camera_->SetTarget(cameraTage);
		camera_->Update();
		ParticleLibrary::GetInstance()->ObjectUpdate();
		ParticleLibrary::GetInstance()->DrawImgui();
		ParticleLibrary::GetInstance()->Update();

#endif
#ifdef NDEBUG
	CursorLimit();

#endif // DEBUG
	ImGuiMan_->End();
	LightUpdate();
#ifndef _PARTICLELIBRARY

	skydome_->Update();
	switch ( scene )
	{
	case GameScene::Title:




		road_->BeforeUpdate();
		title_->Update();
		seenTransition_->Update();
		//player_->Update();
		if ( seenTransition_->ReturnSeenNotEnd() == false )
		{
			if ( input_->TriggerMouse(0) )
			{
				seenTransition_->OnSeenTrans();
				seenFlag = true;
			}
		}
		if ( seenFlag )
		{
			if ( seenTransition_->ReturnSeenTrans() )
			{
				stertCount_->Initialize();
				camera_->SetEye(cameraGame);
				scene = Scene::Game;
				seenFlag = false;
			}
		}
		break;
	case GameScene::Game:

		road_->Update();
		stertCount_->Update();
		if ( stertCount_->GoStert() )
		{
			//camera_->SetEye(cameraGame);
			player_->cameraUpdate();
		}
		seenTransition_->Update();
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
			if ( enemyManager_->Clear() == true )
			{
				scene = Scene::Boss;
			}
			if ( player_->retrunIsDaed() )
			{
				seenTransition_->OnSeenTrans();
				if ( seenTransition_->ReturnSeenTrans() )
				{
					scene = Scene::Over;
				}
			}

			//if ( input_->TriggerKey(DIK_P) )
			//{
			//	player_->OnCollision();
			//}
			//if ( input_->PushKey(DIK_O) )
			//{
			//	seenTransition_->OnSeenTrans();
			//	DemoClear = true;

			//}
			if ( DemoClear )
			{
				if ( seenTransition_->ReturnSeenTrans() )
				{
					scene = Scene::Clear;
				}
			}
		}
		LightData::GetInstance()->Update();
		break;
	case GameScene::Boss:

		bossTime--;
		seenTransition_->Update();
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
			seenTransition_->OnSeenTrans();
			if ( seenTransition_->ReturnSeenTrans() )
			{
				scene = Scene::Clear;
			}

		}
		if ( player_->retrunIsDaed() )
		{
			seenTransition_->OnSeenTrans();
			if ( seenTransition_->ReturnSeenTrans() )
			{
				scene = Scene::Over;

			}
		}
		LightData::GetInstance()->Update();
		break;
	case GameScene::Over:
		road_->roadUpdate();
		gameOverSeen->Update();
		seenTransition_->Update();
		camera_->SetTarget(TargetCamRes);
		if ( seenTransition_->ReturnSeenNotEnd() == false )
		{
			if ( input_->TriggerMouse(0) )
			{
				seenTransition_->OnSeenTrans();
				seenFlag = true;
			}
		}
		if ( seenFlag )
		{
			if ( seenTransition_->ReturnSeenTrans() )
			{
				PhaseReset();
				scene = Scene::Title;
				seenFlag = false;
			}
		}

		break;
	case GameScene::Clear:

		gameClearScene->Update();
		road_->roadUpdate();
		seenTransition_->Update();
		player_->ClearMove();
		if ( seenTransition_->ReturnSeenNotEnd() == false )
		{
			if ( gameClearScene->ClearTrue() )
			{
				if ( input_->TriggerMouse(0) )
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
				PhaseReset();
				scene = Scene::Title;
				seenFlag = false;
			}
		}
		LightData::GetInstance()->Update();
		break;

	default:
		break;
	}

#endif // DEBUG

}
void GameScene::Draw()
{
	/*direLight->Draw(dxCommon_->GetCommandList(),1);*/
	Object3d::PreDraw(dxCommon_->GetCommandList());


#ifndef _PARTICLELIBRARY                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              
	                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                    

	//player_->Draw();

	switch ( scene )
	{
	case GameScene::Title:
		skydome_->Draw();
		road_->Draw();
		/*player_->Draw();*/
		title_->Draw();
		break;
	case GameScene::Game:
		skydome_->Draw();
		road_->Draw();
		player_->Draw();

		enemyManager_->Draw();
		enemyManager_->ParticleDraw();
		break;
	case GameScene::Boss:
		skydome_->Draw();
		road_->Draw();
		player_->Draw();
		if ( bossTime < 0 )
		{
			enemyManager_->BossDraw();
		}
		enemyManager_->ParticleDraw();
		break;
	case GameScene::Over:
		skydome_->Draw();
		road_->Draw();
		gameOverSeen->Draw();
		gameOverSeen->DrawParticle();
		break;
	case GameScene::Clear:
		skydome_->Draw();
		road_->Draw();
		gameClearScene->Draw();
		player_->Draw();

		break;

	case GameScene::Editor:
		skydome_->Draw();
		road_->Draw();
		StageEditor::GetInstance()->Draw();

		break;

	default:
		break;
	}
#endif // !_PARTICLELIBRARY
#ifdef _PARTICLELIBRARY
	//skydome_->Draw();
	//road_->Draw();
	ParticleLibrary::GetInstance()->ObjectDraw();
	ParticleLibrary::GetInstance()->PDraw();
#endif // _DEBUG




	player_->ParticleDraw();
	Object3d::PostDraw();
	ImGuiMan_->Draw();
#ifdef _PARTICLELIBRARY

#endif // _DEBUG
#ifndef _PARTICLELIBRARY
	switch ( scene )
	{
	case GameScene::Title:
		player_->DrawFbx();
		break;
	case GameScene::Game:
		player_->DrawFbx();
		break;
	case GameScene::Boss:
		player_->DrawFbx();
		break;
	case GameScene::Over:
		break;
	case GameScene::Clear:
		player_->DrawFbx();

		break;
	default:
		break;
	}


	switch ( scene )
	{
	case GameScene::Title:
		stert_->Draw();
		break;
	case GameScene::Game:
		if ( stertCount_->stertEnd() )
		{
			player_->DrawUI();
			operation_->Draw();
			cameraMoveOps_->Draw();
			moveTute_->Draw();
		}
		enemyManager_->DrawUI();
		stertCount_->Draw();
		//ImGuiMan->Draw();
		break;
	case GameScene::Boss:
		operation_->Draw();
		cameraMoveOps_->Draw();
		moveTute_->Draw();
		player_->DrawUI();
		enemyManager_->DrawUI();
		break;
	case GameScene::Over:
		gameOver_->Draw();
		stert_->Draw();
		break;
	case GameScene::Clear:
		//gameClear_->Draw();
		gameClearScene->SpriteDraw();
		if ( gameClearScene->ClearTrue() )
		{
			stert_->Draw();
		}
		break;
	default:
		break;
	}
	seenTransition_->Draw();


#endif // !_PARTICLELIBRARY
	

}


void GameScene::Finalize()
{

	audio_->Finalize();


}


void GameScene::PhaseReset()
{
	//gameClear->SetPozition({ winApp_->window_width / 2,-30 });
	//自キャラの初期化
	enemyManager_->EnemyReset();
	//LightData::GetInstance()->Update();
	LightData::GetInstance()->GetLightGroupData()->ResetCircleShadow();
	//LightData::GetInstance()->Destroy();
	//LightData::GetInstance()->StaticInitialize();
	DemoClear = false;
	railCamera->Reset();
	bossTime = 10;
	player_->Reset();

	road_->Reset();
	camera_->SetTarget(TargetCamRes);
	camera_->SetEye(cameraTitle);
	camera_->Update();
	title_->Reset();
	title_->Update();
	stertCount_->Reset();
	gameClearScene->Reset();
}


void GameScene::CursorLimit()
{
	GetClipCursor(&rcOldClip);
	GetWindowRect(winApp_->GetHwnd(),&rcClip);
	ClipCursor(&rcClip);
	ClipCursor(&rcOldClip);
}

void GameScene::LightUpdate()
{
	lightGroupNon->Update();
}


