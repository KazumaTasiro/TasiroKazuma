#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete player_;
	//delete model1;
}
void GameScene::Initialize(WinApp* winApp, DirectXCommon* dxcomon, Input* input_)
{
	assert(dxcomon);
	assert(input_);

	input = input_;

	winApp_ = winApp;

	dxCommon_ = dxcomon;

	audio = new Audio();
	audio->Initialize();
	
	// カメラ生成
	camera = new Camera(WinApp::window_width, WinApp::window_height);
	camera->SetEye({ 0,0,-100 });
	Object3d::SetCamera(camera);

	ImGuiMan = new ImGuiManager();
	ImGuiMan->Initialize(winApp, dxCommon_);

	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon_);
	spriteCommon->LoadTexture(0, "Reticle.png");
	spriteCommon->LoadTexture(1, "ReticleLock.png");
	spriteCommon->LoadTexture(2, "EnemyLock.png");
	spriteCommon->LoadTexture(3, "stert.png");

	skydome = new Skydome();
	skydome->Initalize();

	stert = new Sprite();
	stert->Initialize(spriteCommon, 3);
	stert->SetPozition({ winApp->window_width/2,winApp->window_height/2 });

	//Object3dFbx::SetDevice(dxCommon_->GetDevice());
	//Object3dFbx::SetCamera(camera);
	//Object3dFbx::CreateGraphicsPipeline();
	////モデル名を指定してファイルに読み込み
	//model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");


	//object1 = new Object3dFbx;
	//object1->Initialize();
	//object1->SetModel(model1);
	//object1->PlayAnimation();
	
	player_ = new Player();
	player_->Initialize(spriteCommon, input,winApp);

	
	
	
	enemyManager = new EnemyManager();
	enemyManager->Initialize(dxCommon_,input,spriteCommon,camera);

	enemyManager->SetGameScene(this);

	scene = Scene::Title;

}
void GameScene::Update()
{
	ImGuiMan->Bigin();

	ImGui::SetWindowSize({ 500,100 });


	//デモウィンドウの表示ON
	ImGui::ShowDemoWindow();

	ImGuiMan->End();
	switch (scene)
	{
	case GameScene::Title:
		if (input->TriggerKey(DIK_Q)) {
			scene = Scene::Game;
		}
		break;
	case GameScene::Game:
		enemyManager->SetPlayer(player_);
		enemyManager->Update();
		enemyManager->EnemyCollision(player_);



		//for (std::unique_ptr<Enemy>& enemy : enemy_) {
		//	enemy->OnCollision();
		//}

		//camera->SetEye({ 0,-300,300, });
		camera->Update();
		player_->Update();
		if (input->TriggerKey(DIK_Q)) {
			scene = Scene::GameClear;
		}
		if (input->TriggerKey(DIK_E)) {
			scene = Scene::GameOver;
		}
		break;
	case GameScene::GameOver:
		if (input->TriggerKey(DIK_Q)) {
			scene = Scene::Title;
		}
		PhaseReset();
		break;
	case GameScene::GameClear:
		if (input->TriggerKey(DIK_Q)) {
			scene = Scene::Title;
		}
		PhaseReset();
		break;
	default:
		break;
	}

	

}
void GameScene::Draw()
{
	Object3d::PreDraw(dxCommon_->GetCommandList());
	switch (scene)
	{
	case GameScene::Title:
		break;
	case GameScene::Game:
	
	
		enemyManager->Draw();
		player_->Draw();
		skydome->Draw();

		break;
	case GameScene::GameOver:
		break;
	case GameScene::GameClear:
		break;
	default:
		break;
	}

	Object3d::PostDraw();
	



	spriteCommon->PreDraw();

	switch (scene)
	{
	case GameScene::Title:
		stert->Draw();
		break;
	case GameScene::Game:
		player_->DrawUI();
		enemyManager->DrawUI();
		ImGuiMan->Draw();
		break;
	case GameScene::GameOver:
		stert->Draw();
		break;
	case GameScene::GameClear:
		stert->Draw();
		break;
	default:
		break;
	}

	

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

void GameScene::CheckAllCollisions()
{

}

void GameScene::PhaseReset()
{
	//自キャラの初期化
	player_->Reset();
	enemyManager->EnemyReset();
}


