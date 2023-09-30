#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete player_;
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

	// �J��������
	camera = new Camera(WinApp::window_width, WinApp::window_height);
	camera->SetEye({ 0,0,-10 });

	camera->Update();
	Object3d::SetCamera(camera);
	ParticleManager::SetCamera(camera);

	ImGuiMan = new ImGuiManager();
	ImGuiMan->Initialize(winApp, dxCommon_);

	//�X�v���C�g���ʕ����̏�����
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon_);
	spriteCommon->LoadTexture(0, "Reticle.png");
	spriteCommon->LoadTexture(1, "ReticleLock.png");
	spriteCommon->LoadTexture(2, "EnemyLock.png");
	spriteCommon->LoadTexture(3, "stert.png");
	spriteCommon->LoadTexture(4, "GameBlind.png");
	spriteCommon->LoadTexture(5, "GameClear.png");
	spriteCommon->LoadTexture(6, "GameOver.png");

	skydome = new Skydome();
	skydome->Initalize();

	stert = new Sprite();
	stert->Initialize(spriteCommon, 3);
	stert->SetPozition({ winApp->window_width / 2,winApp->window_height / 2 + 50 });

	gameClear = new Sprite();
	gameClear->Initialize(spriteCommon, 5);
	gameClear->SetPozition({ winApp->window_width / 2,(winApp->window_height / 2) - 80 });
	spriteEnd = { winApp->window_width / 2,winApp->window_height / 2 };
	gameOver = new Sprite();
	gameOver->Initialize(spriteCommon, 6);
	gameOver->SetPozition({ winApp->window_width / 2,(winApp->window_height / 2 )-80 });

	blind = new Sprite();
	blind->Initialize(spriteCommon, 4);
	blind->SetPozition({ winApp->window_width / 2,winApp->window_height / 2 });
	blind->SetSize({ winApp->window_width,winApp->window_height });
	blind->SetColor({ blind->GetColor().x,blind->GetColor().y,blind->GetColor().z,blindW });

	road = new Road();
	road->Initialize();


	//Object3dFbx::SetDevice(dxCommon_->GetDevice());
	//Object3dFbx::SetCamera(camera);
	//Object3dFbx::CreateGraphicsPipeline();
	////���f�������w�肵�ăt�@�C���ɓǂݍ���
	//model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");


	//object1 = new Object3dFbx;
	//object1->Initialize();
	//object1->SetModel(model1);
	//object1->PlayAnimation();

	ParticleMana = new ParticleManager();
	ParticleMana->Initialize();
	ParticleMana->LoadTexture("Explosion.png");

	player_ = new Player();
	player_->Initialize(spriteCommon, input, winApp, ParticleMana);

	enemyManager = new EnemyManager();
	enemyManager->Initialize(dxCommon_, input, spriteCommon, camera, ParticleMana);

	enemyManager->SetGameScene(this);
	enemyManager->SetPlayer(player_);
	PhaseReset();
	//player_->Update();



	scene = Scene::Title;


}
void GameScene::Update()
{
	ImGuiMan->Bigin();

	ImGui::SetWindowSize({ 500,100 });

	//�f���E�B���h�E�̕\��ON
	ImGui::ShowDemoWindow();

	ImGui::SliderFloat("posz", &playPos, -100, 0);
	//player_->SetPos({ player_->GetWorldPosition().x ,player_->GetWorldPosition().y,playPos });
	ImGuiMan->End();
	switch (scene)
	{
	case GameScene::Title:
		road->BeforeUpdate();
		//player_->Update();
		if (input->TriggerKey(DIK_Q)) {
			blindFlag = true;
		}
		if (input->TriggerKey(DIK_B)) {
			scene = Scene::Boss;
		}
		if (blindFlag) {
			blindTime--;
			if (blindW < 1) {
				blindW += (1.0f / blindTimeReset);
			}
			blind->SetColor({ blind->GetColor().x,blind->GetColor().y,blind->GetColor().z,blindW });
			blind->Update();
		}
		if (blindTime <= -10) {
			scene = Scene::Game;
		}

		if (input->PushKey(DIK_L)) {
			scene = Scene::GameClear;
		}
		if (input->PushKey(DIK_P)) {
			scene = Scene::GameOver;
		}
		break;
	case GameScene::Game:
		if (blindFlag) {
			blindTime++;
			if (blindTime >= 0) {
				if (blindW > 0) {
					blindW -= (1.0f / blindTimeReset);
				}
			}
			blind->SetColor({ blind->GetColor().x,blind->GetColor().y,blind->GetColor().z,blindW });
			blind->Update();
		}
		if (blindTime >= blindTimeReset) {
			blindFlag = false;
		}
		camera->SetTarget({ (player_->GetReticlePos().x / 100),(player_->GetReticlePos().y / 100),camera->GetTarget().z });
		road->Update();
		enemyManager->SetPlayer(player_);
		enemyManager->Update();
		enemyManager->EnemyCollision(player_);



		//for (std::unique_ptr<Enemy>& enemy : enemy_) {
		//	enemy->OnCollision();
		//}

		//camera->SetEye({ 0,-300,300, });
		camera->SetTarget({ (player_->GetReticlePos().x / 100),(player_->GetReticlePos().y / 100),camera->GetTarget().z });
		camera->Update();
		player_->Update();
		if (enemyManager->Clear() == true) {
			scene = Scene::Boss;
		}
		if (player_->retrunIsDaed()) {
			scene = Scene::GameOver;
		}
		break;
	case GameScene::Boss:
		camera->SetTarget({ (player_->GetReticlePos().x / 100),(player_->GetReticlePos().y / 100),camera->GetTarget().z });
		road->Update();
		enemyManager->SetPlayer(player_);
		enemyManager->BossUpdate();
		enemyManager->Update();
		enemyManager->EnemyCollision(player_);
		camera->SetTarget({ (player_->GetReticlePos().x / 100),(player_->GetReticlePos().y / 100),camera->GetTarget().z });
		camera->Update();
		player_->Update();
		if (enemyManager->BossClear()) {
			scene = Scene::GameClear;
		}
		if (player_->retrunIsDaed()) {
			scene = Scene::GameOver;
		}
		break;
	case GameScene::GameOver:
		camera->SetTarget({ 0,0,0 });
		if (input->TriggerKey(DIK_Q)) {
			PhaseReset();
			scene = Scene::Title;
		}

		break;
	case GameScene::GameClear:
		camera->SetTarget({ 0,0,0 });
		player_->ClearMove();
		if (input->TriggerKey(DIK_Q)) {
			PhaseReset();
			scene = Scene::Title;
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

	switch (scene)
	{
	case GameScene::Title:
		player_->Draw();
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




	spriteCommon->PreDraw();

	switch (scene)
	{
	case GameScene::Title:
		stert->Draw();
		blind->Draw();
		break;
	case GameScene::Game:
		player_->DrawUI();
		enemyManager->DrawUI();
		blind->Draw();
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

	//ImGuiMan->Draw();

}


void GameScene::Finalize()
{
	ImGuiMan->Finalize();
	audio->Finalize();

	delete audio;
	//ImGui�̊J��
	delete ImGuiMan;

	//3D���f���J��
	delete model;

	//�X�v���C�g�̊J��
	delete spriteCommon;

}

void GameScene::CheckAllCollisions()
{

}

void GameScene::ClearSpriteUpdate()
{
	//if (gameClear->GetPosition().y < spriteEnd.y) {
	//	gameClear->SetPozition({ gameClear->GetPosition().x, gameClear->GetPosition().y + 30 });
	//}
	//else
	//{
	//	gameClear->SetPozition(spriteEnd);
	//}
}

void GameScene::GameOverSpriteUpdate()
{
}

void GameScene::PhaseReset()
{
	//gameClear->SetPozition({ winApp_->window_width / 2,-30 });
	//���L�����̏�����
	player_->Reset();
	enemyManager->EnemyReset();
	road->Reset();
}


