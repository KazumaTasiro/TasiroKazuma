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
	camera_ = new Camera(winApp_->window_width,winApp_->window_height);
	camera_->SetEye(cameraTitle);

	camera_->Update();
	Object3d::SetCamera(camera_);
	ParticleManager::SetCamera(camera_);

	ImGuiMan_ = new ImGuiManager();
	ImGuiMan_->Initialize(dxCommon_);

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


	skydome_ = new Skydome();
	skydome_->Initalize();

	stert_ = new Sprite();
	stert_->Initialize(spriteCommon_,3);
	stert_->SetPozition({ winApp_->window_width / 2,winApp_->window_height / 2 + 50 });

	gameClear_ = new Sprite();
	gameClear_->Initialize(spriteCommon_,5);
	gameClear_->SetPozition({ winApp_->window_width / 2,( winApp_->window_height / 2 ) - 80 });
	spriteEnd_ = { winApp_->window_width / 2,winApp_->window_height / 2 };
	gameOver_ = new Sprite();
	gameOver_->Initialize(spriteCommon_,6);
	gameOver_->SetPozition({ winApp_->window_width / 2,( winApp_->window_height / 2 ) - 80 });



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

	enemyManager_ = new EnemyManager();
	enemyManager_->Initialize(spriteCommon_,camera_,ParticleMana_);

	enemyManager_->SetGameScene(this);
	enemyManager_->SetPlayer(player_);

	//player_->Update();
	seenTransition_ = new SeenTransition();
	seenTransition_->Initialize(spriteCommon_);

	title_ = new Titles();
	title_->Initialize();

	
	scene = Scene::Title;
	stertCount_ = new StertCount();
	stertCount_->SetCamera(camera_);

	PhaseReset();
}
void GameScene::Update()
{
	ImGuiMan_->Bigin();

	ImGui::SetWindowSize({ 500,100 });

	//デモウィンドウの表示ON
	ImGui::ShowDemoWindow();

	ImGui::SliderFloat("posz",&playPos,-100,0);
	//player_->SetPos({ player_->GetWorldPosition().x ,player_->GetWorldPosition().y,playPos });
	ImGuiMan_->End();
	switch ( scene )
	{
	case GameScene::Title:
		road_->BeforeUpdate();
		title_->Update();
		seenTransition_->Update();
		//player_->Update();
		if ( seenTransition_->ReturnSeenNotEnd() == false )
		{
			if ( input_->TriggerKey(DIK_Q) )
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
		if ( input_->TriggerKey(DIK_B) )
		{
			scene = Scene::Boss;
			enemyManager_->bossSeenTest();
		}

		if ( input_->PushKey(DIK_L) )
		{
			scene = Scene::GameClear;
		}
		if ( input_->PushKey(DIK_P) )
		{
			scene = Scene::GameOver;
		}
		break;
	case GameScene::Game:

		road_->Update();
		stertCount_->Update();
		seenTransition_->Update();
		camera_->Update();
		player_->Update();
		if ( stertCount_->stertEnd() )
		{
			camera_->SetEye(cameraGame);
			camera_->SetTarget({ ( player_->GetReticlePos().x / 100 ),( player_->GetReticlePos().y / 100 ),camera_->GetTarget().z });
			player_->Update();
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
					scene = Scene::GameOver;
				}
			}

			if ( input_->PushKey(DIK_P) )
			{
				player_->OnCollision();
			}
		}
		break;
	case GameScene::Boss:
		seenTransition_->Update();
		camera_->SetTarget({ ( player_->GetReticlePos().x / 100 ),( player_->GetReticlePos().y / 100 ),camera_->GetTarget().z });
		road_->Update();
		enemyManager_->SetPlayer(player_);
		enemyManager_->BossUpdate();
		enemyManager_->Update();
		enemyManager_->EnemyCollision(player_);
		camera_->SetTarget({ ( player_->GetReticlePos().x / 100 ),( player_->GetReticlePos().y / 100 ),camera_->GetTarget().z });
		camera_->Update();
		player_->Update();
		if ( enemyManager_->BossClear() )
		{
			seenTransition_->OnSeenTrans();
			if ( seenTransition_->ReturnSeenTrans() )
			{
				scene = Scene::GameClear;
			}

		}
		if ( player_->retrunIsDaed() )
		{
			seenTransition_->OnSeenTrans();
			if ( seenTransition_->ReturnSeenTrans() )
			{
				scene = Scene::GameOver;

			}
		}

		break;
	case GameScene::GameOver:
		seenTransition_->Update();
		camera_->SetTarget({ 0,0,0 });
		if ( seenTransition_->ReturnSeenNotEnd() == false )
		{
			if ( input_->TriggerKey(DIK_Q) )
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
	case GameScene::GameClear:
		seenTransition_->Update();
		camera_->SetTarget({ 0,0,0 });
		player_->ClearMove();
		if ( seenTransition_->ReturnSeenNotEnd() == false )
		{
			if ( input_->TriggerKey(DIK_Q) )
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
	default:
		break;
	}



}
void GameScene::Draw()
{
	Object3d::PreDraw(dxCommon_->GetCommandList());


	skydome_->Draw();
	road_->Draw();
	//player_->Draw();

	switch ( scene )
	{
	case GameScene::Title:
		/*player_->Draw();*/
		title_->Draw();
		break;
	case GameScene::Game:
		player_->Draw();

		enemyManager_->Draw();
		enemyManager_->ParticleDraw();
		break;
	case GameScene::Boss:
		player_->Draw();
		enemyManager_->BossDraw();
		enemyManager_->ParticleDraw();
		break;
	case GameScene::GameOver:
		break;
	case GameScene::GameClear:
		player_->Draw();

		break;
	default:
		break;
	}


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
	case GameScene::GameOver:
		break;
	case GameScene::GameClear:
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
		player_->DrawUI();
		enemyManager_->DrawUI();
		stertCount_->Draw();
		//ImGuiMan->Draw();
		break;
	case GameScene::Boss:
		player_->DrawUI();
		enemyManager_->DrawUI();
		break;
	case GameScene::GameOver:
		gameOver_->Draw();
		stert_->Draw();
		break;
	case GameScene::GameClear:
		gameClear_->Draw();
		stert_->Draw();
		break;
	default:
		break;
	}
	seenTransition_->Draw();
	//ImGuiMan->Draw();

}


void GameScene::Finalize()
{
	ImGuiMan_->Finalize();
	audio_->Finalize();

	delete audio_;
	//ImGuiの開放
	delete ImGuiMan_;

	//3Dモデル開放
	delete model_;

	//スプライトの開放
	delete spriteCommon_;

}


void GameScene::PhaseReset()
{
	//gameClear->SetPozition({ winApp_->window_width / 2,-30 });
	//自キャラの初期化
	player_->Reset();
	enemyManager_->EnemyReset();
	road_->Reset();
	camera_->SetEye(cameraTitle);
	camera_->Update();
	title_->Reset();
	title_->Update();
	stertCount_->Reset();
}

void GameScene::TitleReset()
{

}


