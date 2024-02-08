#include "GameScene.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	delete player_;
}
void GameScene::Initialize(DirectXCommon* dxcomon)
{
	assert(dxcomon);


	input_ = Input::GetInstance();

	winApp_ = WinApp::GetInstance();

	dxCommon_ = dxcomon;

	audio_ = new Audio();
	audio_->Initialize();

	// カメラ生成
	camera_ = new Camera(WinApp::window_width,WinApp::window_height);
	camera_->SetEye(cameraTitle);

	camera_->Update();
	Object3d::SetCamera(camera_);
	ParticleManager::SetCamera(camera_);

	ImGuiMan_ = ImGuiManager::GetInstance();

	//スプライト共通部分の初期化
	spriteCommon_ = new SpriteCommon;
	spriteCommon_->Initialize(dxCommon_);
	spriteCommon_->LoadTexture(0,"Reticle.png");
	spriteCommon_->LoadTexture(1,"ReticleLock.png");
	spriteCommon_->LoadTexture(2,"EnemyLock.png");
	spriteCommon_->LoadTexture(3,"stert.png");
	spriteCommon_->LoadTexture(4,"GameBlind.png");
	spriteCommon_->LoadTexture(5,"GameClear.png");
	spriteCommon_->LoadTexture(6,"GameOver.png");
	spriteCommon_->LoadTexture(7,"GameBlindFaceOver1.png");
	spriteCommon_->LoadTexture(8,"GameBlindFaceOver2.png");
	spriteCommon_->LoadTexture(9,"GameBlindFaceOver3.png");
	spriteCommon_->LoadTexture(10,"GameBlindFaceUnder1.png");
	spriteCommon_->LoadTexture(11,"GameBlindFaceUnder2.png");
	spriteCommon_->LoadTexture(12,"GameBlindFaceUnder3.png");
	spriteCommon_->LoadTexture(13,"Time3.png");
	spriteCommon_->LoadTexture(14,"Time2.png");
	spriteCommon_->LoadTexture(15,"Time1.png");
	spriteCommon_->LoadTexture(16,"TimeGo.png");
	spriteCommon_->LoadTexture(17,"playerHPNone.png");
	spriteCommon_->LoadTexture(18,"playerHPMax.png");
	spriteCommon_->LoadTexture(19,"Operation.png");
	spriteCommon_->LoadTexture(20,"CameraMove.png");
	spriteCommon_->LoadTexture(21,"CengeCameraE.png");
	spriteCommon_->LoadTexture(22,"CengeCmaraQ.png");
	spriteCommon_->LoadTexture(23,"damageEf.png");
	spriteCommon_->LoadTexture(24,"MoveTuto.png");


	skydome_ = new Skydome();
	skydome_->Initalize();

	stert_ = new Sprite();
	stert_->Initialize(spriteCommon_,three);
	stert_->SetPozition({ winApp_->window_width / 2,winApp_->window_height / 2 + gamestertUp });


	gameClear_ = new Sprite();
	gameClear_->Initialize(spriteCommon_,five);
	gameClear_->SetPozition({ winApp_->window_width / 2,( winApp_->window_height / 2 ) - gameOverUp });
	spriteEnd_ = { winApp_->window_width / 2,winApp_->window_height / 2 };
	gameOver_ = new Sprite();
	gameOver_->Initialize(spriteCommon_,six);
	gameOver_->SetPozition({ winApp_->window_width / 2,( winApp_->window_height / 2 ) - gameOverUp });
	operation_ = new Sprite();
	operation_->Initialize(spriteCommon_,19);
	operation_->SetPozition({ WinApp::window_width - operationPos.x,WinApp::window_height - operationPos.y });
	//operation_->SetAnchorPoint({ 0,0 })
	cameraMoveOps_= new Sprite();
	cameraMoveOps_->Initialize(spriteCommon_,20);
	cameraMoveOps_->SetPozition({ WinApp::window_width - operationPos.x,WinApp::window_height - (operationPos.y*3) });
	moveTute_ = new Sprite();
	moveTute_->Initialize(spriteCommon_,24);
	moveTute_->SetPozition({ WinApp::window_width - operationPos.x,WinApp::window_height - ( operationPos.y * 5 ) });

	road_ = new Road();
	road_->Initialize();


	Object3dFbx::SetDevice(dxCommon_->GetDevice());
	Object3dFbx::SetCamera(camera_);
	Object3dFbx::CreateGraphicsPipeline();
	////モデル名を指定してファイルに読み込み
	//model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");


	//object1 = new Object3dFbx;
	//object1->Initialize();
	//object1->SetModel(model1);
	//object1->PlayAnimation();

	ParticleMana_ = new ParticleManager();
	ParticleMana_->Initialize();
	ParticleMana_->LoadTexture("Explosion.png");

	player_ = new Player();
	player_->Initialize(spriteCommon_,ParticleMana_);
	player_->SetCamera(camera_);

	enemyManager_ = new EnemyManager();
	enemyManager_->Initialize(spriteCommon_,camera_,ParticleMana_);

	enemyManager_->SetGameScene(this);
	enemyManager_->SetPlayer(player_);

	//player_->Update();
	seenTransition_ = new SeenTransition();
	seenTransition_->Initialize(spriteCommon_);

	gameOverSeen = new GameOver();
	gameOverSeen->Initialize();
	gameOverSeen->SetCamera(camera_);

	gameClearScene = new GameClear();
	gameClearScene->SetCamera(camera_);
	gameClearScene->Initialize(spriteCommon_);

	title_ = new Titles();
	title_->Initialize();


	scene = Scene::Title;
	stertCount_ = new StertCount();
	stertCount_->SetCamera(camera_);
	railCamera = new RailCamera();
	railCamera->SetCamera(camera_);
	railCamera->SetPlayer(player_);
	railCamera->Initialize();
	railCamera->Update();

	lightGroupNon = LightGroup::Create();

	lightGroupUse = LightGroup::Create();

	//direLight = DirectionalLight::Create();

	//lightGroupUse->SetDirLightActive(0,true);
	//lightGroupUse->SetPointLightActive(0,true);
	//lightGroupUse->SetCircleShadowActive(0,true);

	Object3d::SetLightNon(lightGroupNon);
	Object3d::SetLightUse(lightGroupUse);
	PhaseReset();

	
	//pointLight = LightData::GetInstance()->AddPointLight(pointLightPos,pointLightColor,pointLightAtten);

}
void GameScene::Update()
{

	//direLight->Update();
	ImGuiMan_->Bigin();
#ifdef _DEBUG
	lightGroupUse->Update();


	//ImGui::SliderFloat3("circleShadowAttenX",&circleShadowAtten.x,0,2);
	//ImGui::SliderFloat2("circleShadowFactorAngleX",&circleShadowFactorAngle.x,0,2);
	//player_->worldTransform_->lightGroup->SetCircleShadowDir(0,circleShadowDir);
	//player_->worldTransform_->lightGroup->SetCircleShadowAtten(0,circleShadowAtten);
	//player_->worldTransform_->lightGroup->SetCircleShadowFactorAngle(0,circleShadowFactorAngle);
	/*ImGui::ShowDemoWindow();*/
	/*player_->ImguiDraw();*/
	//ImGui::SliderFloat3("pointLightPos",&pointLightPos.x,-100,100);
	//ImGui::SliderFloat3("pointLightColor",&pointLightColor.x,0,1);
	//ImGui::SliderFloat3("pointLightAtten",&pointLightAtten.x,0,1);
	//LightData::GetInstance()->UpdatePointLight(pointLight,pointLightPos,pointLightColor,pointLightAtten);

#endif
#ifdef _PARTICLELIBRARY

		//ImGui::SliderFloat3("pointLightPos",&pointLightPos.x,-100,100);
		//ImGui::SliderFloat3("pointLightColor",&pointLightColor.x,0,1);
		//ImGui::SliderFloat3("pointLightAtten",&pointLightAtten.x,0,1);
		ImGui::SliderFloat3("cameraEye",&cameraImgui.x,-30,30);

		camera_->SetEye(cameraImgui);
		camera_->Update();
		ParticleLibrary::GetInstance()->DrawImgui();
		ParticleLibrary::GetInstance()->Update();

		break;

#endif
#ifdef NDEBUG
	CursorLimit();

#endif // DEBUG

			//デモウィンドウの表示ON


	/*enemyManager_->ImguiUpdate();*/
	//player_->SetPos({ player_->GetWorldPosition().x ,player_->GetWorldPosition().y,playPos });
	ImGuiMan_->End();
	LightUpdate();
	skydome_->Update();
	switch ( scene )
	{
	case GameScene::Title:
#ifdef _DEBUG
#endif // DEBUG


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
				stertCount_->Initialize(spriteCommon_);
				camera_->SetEye(cameraGame);
				scene = Scene::Game;
				seenFlag = false;
			}
		}
		//if ( input_->TriggerKey(DIK_B) )
		//{
		//	scene = Scene::Boss;
		//	enemyManager_->bossSeenTest();
		//}

		//if ( input_->PushKey(DIK_L) )
		//{
		//	scene = Scene::GameClear;
		//}
		//if ( input_->PushKey(DIK_P) )
		//{
		//	scene = Scene::GameOver;
		//}
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
			enemyManager_->SetPlayer(player_);
			enemyManager_->Update();
			enemyManager_->EnemyCollision(player_);
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
		enemyManager_->SetPlayer(player_);
		enemyManager_->BossUpdate();
		enemyManager_->Update();
		enemyManager_->EnemyCollision(player_);
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



}
void GameScene::Draw()
{
	/*direLight->Draw(dxCommon_->GetCommandList(),1);*/
	Object3d::PreDraw(dxCommon_->GetCommandList());



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



		break;
	default:
		break;
	}
#ifdef _PARTICLELIBRARY
	//skydome_->Draw();
	//road_->Draw();
	ParticleLibrary::GetInstance()->Draw();
#endif // _DEBUG
	player_->ParticleDraw();
	Object3d::PostDraw();

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
	ImGuiMan_->Draw();


	

}


void GameScene::Finalize()
{

	audio_->Finalize();

	delete audio_;

	//3Dモデル開放
	delete model_;

	//スプライトの開放
	delete spriteCommon_;

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

	//lightGroupNon->SetCircleShadowDir(0,circleShadowDir);
	//lightGroupNon->SetCircleShadowCasterPos(0,{0,0,0});
	//lightGroupNon->SetCircleShadowAtten(0,circleShadowAtten);
	//lightGroupNon->SetCircleShadowFactorAngle(0,circleShadowFactorAngle);
}


