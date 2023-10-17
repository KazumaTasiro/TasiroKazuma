#include "GameScene.h"

GameScene::GameScene()
{

}

GameScene::~GameScene()
{
	delete player_;
}
void GameScene::Initialize(WinApp* winApp,DirectXCommon* dxcomon,Input* input_)
{
	assert(dxcomon);
	assert(input_);
	assert(winApp);

	input = input_;

	winApp_ = winApp;

	dxCommon_ = dxcomon;

	audio = new Audio();
	audio->Initialize();

	// カメラ生成
	camera = new Camera(WinApp::window_width,WinApp::window_height);
	camera->SetEye(cameraTitle);

	camera->Update();
	Object3d::SetCamera(camera);
	ParticleManager::SetCamera(camera);

	ImGuiMan = new ImGuiManager();
	ImGuiMan->Initialize(winApp,dxCommon_);

	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon_);
	spriteCommon->LoadTexture(0,"Reticle.png");
	spriteCommon->LoadTexture(1,"ReticleLock.png");
	spriteCommon->LoadTexture(2,"EnemyLock.png");
	spriteCommon->LoadTexture(3,"stert.png");
	spriteCommon->LoadTexture(4,"GameBlind.png");
	spriteCommon->LoadTexture(5,"GameClear.png");
	spriteCommon->LoadTexture(6,"GameOver.png");
	spriteCommon->LoadTexture(7,"GameBlindFaceOver1.png");
	spriteCommon->LoadTexture(8,"GameBlindFaceOver2.png");
	spriteCommon->LoadTexture(9,"GameBlindFaceOver3.png");
	spriteCommon->LoadTexture(10,"GameBlindFaceUnder1.png");
	spriteCommon->LoadTexture(11,"GameBlindFaceUnder2.png");
	spriteCommon->LoadTexture(12,"GameBlindFaceUnder3.png");


	skydome = new Skydome();
	skydome->Initalize();

	stert = new Sprite();
	stert->Initialize(spriteCommon,3);
	stert->SetPozition({ winApp->window_width / 2,winApp->window_height / 2 + 50 });

	gameClear = new Sprite();
	gameClear->Initialize(spriteCommon,5);
	gameClear->SetPozition({ winApp->window_width / 2,( winApp->window_height / 2 ) - 80 });
	spriteEnd = { winApp->window_width / 2,winApp->window_height / 2 };
	gameOver = new Sprite();
	gameOver->Initialize(spriteCommon,6);
	gameOver->SetPozition({ winApp->window_width / 2,( winApp->window_height / 2 ) - 80 });



	road = new Road();
	road->Initialize();


	Object3dFbx::SetDevice(dxCommon_->GetDevice());
	Object3dFbx::SetCamera(camera);
	Object3dFbx::CreateGraphicsPipeline();
	////モデル名を指定してファイルに読み込み
	//model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");


	//object1 = new Object3dFbx;
	//object1->Initialize();
	//object1->SetModel(model1);
	//object1->PlayAnimation();

	ParticleMana = new ParticleManager();
	ParticleMana->Initialize();
	ParticleMana->LoadTexture("Explosion.png");

	player_ = new Player();
	player_->Initialize(spriteCommon,input,winApp_,ParticleMana);

	enemyManager = new EnemyManager();
	enemyManager->Initialize(input,spriteCommon,camera,ParticleMana);

	enemyManager->SetGameScene(this);
	enemyManager->SetPlayer(player_);

	//player_->Update();
	seenTransition = new SeenTransition();
	seenTransition->Initialize(spriteCommon);

	title = new Titles();
	title->Initialize();

	PhaseReset();
	scene = Scene::Title;


}
void GameScene::Update()
{
	ImGuiMan->Bigin();

	ImGui::SetWindowSize({ 500,100 });

	//デモウィンドウの表示ON
	ImGui::ShowDemoWindow();

	ImGui::SliderFloat("posz",&playPos,-100,0);
	//player_->SetPos({ player_->GetWorldPosition().x ,player_->GetWorldPosition().y,playPos });
	ImGuiMan->End();
	switch ( scene )
	{
	case GameScene::Title:
		road->BeforeUpdate();
		title->Update();
		seenTransition->Update();
		//player_->Update();
		if ( seenTransition->ReturnSeenNotEnd() == false )
		{
			if ( input->TriggerKey(DIK_Q) )
			{
				seenTransition->OnSeenTrans();
				seenFlag = true;
			}
		}
		if ( seenFlag )
		{
			if ( seenTransition->ReturnSeenTrans() )
			{
				camera->SetEye(cameraGame);
				scene = Scene::Game;
				seenFlag = false;
			}
		}
		if ( input->TriggerKey(DIK_B) )
		{
			scene = Scene::Boss;
			enemyManager->bossSeenTest();
		}

		if ( input->PushKey(DIK_L) )
		{
			scene = Scene::GameClear;
		}
		if ( input->PushKey(DIK_P) )
		{
			scene = Scene::GameOver;
		}
		break;
	case GameScene::Game:

		camera->SetTarget({ ( player_->GetReticlePos().x / 100 ),( player_->GetReticlePos().y / 100 ),camera->GetTarget().z });
		road->Update();
		enemyManager->SetPlayer(player_);
		enemyManager->Update();
		enemyManager->EnemyCollision(player_);
		seenTransition->Update();

		camera->SetTarget({ ( player_->GetReticlePos().x / 100 ),( player_->GetReticlePos().y / 100 ),camera->GetTarget().z });
		camera->Update();
		player_->Update();
		if ( enemyManager->Clear() == true )
		{
			scene = Scene::Boss;
		}
		if ( player_->retrunIsDaed() )
		{
			seenTransition->OnSeenTrans();
			if ( seenTransition->ReturnSeenTrans() )
			{
				scene = Scene::GameOver;
			}
		}

		if ( input->PushKey(DIK_P) )
		{
			player_->OnCollision();
		}

		break;
	case GameScene::Boss:
		seenTransition->Update();
		camera->SetTarget({ ( player_->GetReticlePos().x / 100 ),( player_->GetReticlePos().y / 100 ),camera->GetTarget().z });
		road->Update();
		enemyManager->SetPlayer(player_);
		enemyManager->BossUpdate();
		enemyManager->Update();
		enemyManager->EnemyCollision(player_);
		camera->SetTarget({ ( player_->GetReticlePos().x / 100 ),( player_->GetReticlePos().y / 100 ),camera->GetTarget().z });
		camera->Update();
		player_->Update();
		if ( enemyManager->BossClear() )
		{
			seenTransition->OnSeenTrans();
			if ( seenTransition->ReturnSeenTrans() )
			{
				scene = Scene::GameClear;
			}

		}
		if ( player_->retrunIsDaed() )
		{
			seenTransition->OnSeenTrans();
			if ( seenTransition->ReturnSeenTrans() )
			{
				scene = Scene::GameOver;

			}
		}

		break;
	case GameScene::GameOver:
		seenTransition->Update();
		camera->SetTarget({ 0,0,0 });
		if ( seenTransition->ReturnSeenNotEnd() == false )
		{
			if ( input->TriggerKey(DIK_Q) )
			{
				seenTransition->OnSeenTrans();
				seenFlag = true;
			}
		}
		if ( seenFlag )
		{
			if ( seenTransition->ReturnSeenTrans() )
			{
				PhaseReset();
				scene = Scene::Title;
				seenFlag = false;
			}
		}

		break;
	case GameScene::GameClear:
		seenTransition->Update();
		camera->SetTarget({ 0,0,0 });
		player_->ClearMove();
		if ( seenTransition->ReturnSeenNotEnd() == false )
		{
			if ( input->TriggerKey(DIK_Q) )
			{
				seenTransition->OnSeenTrans();
				seenFlag = true;
			}
		}
		if ( seenFlag )
		{
			if ( seenTransition->ReturnSeenTrans() )
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


	skydome->Draw();
	road->Draw();
	//player_->Draw();

	switch ( scene )
	{
	case GameScene::Title:
		/*player_->Draw();*/
		title->Draw();
		break;
	case GameScene::Game:
		player_->Draw();

		enemyManager->Draw();
		enemyManager->ParticleDraw();
		break;
	case GameScene::Boss:
		player_->Draw();
		enemyManager->BossDraw();
		enemyManager->ParticleDraw();
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
		stert->Draw();
		break;
	case GameScene::Game:
		player_->DrawUI();
		enemyManager->DrawUI();
		//ImGuiMan->Draw();
		break;
	case GameScene::Boss:
		player_->DrawUI();
		enemyManager->DrawUI();
		break;
	case GameScene::GameOver:
		gameOver->Draw();
		stert->Draw();
		break;
	case GameScene::GameClear:
		gameClear->Draw();
		stert->Draw();
		break;
	default:
		break;
	}
	seenTransition->Draw();
	//ImGuiMan->Draw();

}


void GameScene::Finalize()
{
	ImGuiMan->Finalize();
	audio->Finalize();

	delete audio;
	//ImGuiの開放
	delete ImGuiMan;

	//3Dモデル開放
	delete model;

	//スプライトの開放
	delete spriteCommon;

}


void GameScene::PhaseReset()
{
	//gameClear->SetPozition({ winApp_->window_width / 2,-30 });
	//自キャラの初期化
	player_->Reset();
	enemyManager->EnemyReset();
	road->Reset();
	camera->SetEye(cameraTitle);
	camera->Update();
	title->Reset();
	title->Update();
}

void GameScene::TitleReset()
{

}


