#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
}
void GameScene::Initialize(WinApp* winApp,DirectXCommon* dxcomon, Input* input_)
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
	camWtf.Initialize();
	camWtf.position = { 0.0f, 3.0f, -8.0f };

	ParticleManager::SetCamera(camera);
	ParticleManager2::SetCamera(camera);
	Object3d::SetCamera(camera);

	ImGuiMan = new ImGuiManager();
	ImGuiMan->Initialize(winApp, dxCommon_);

	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon_);

	spriteCommon->LoadTexture(0, "meemu.jpg");
	spriteCommon->LoadTexture(1, "wakka.jpg");




	
}
void GameScene::Update()
{


	ImGuiMan->Bigin();

	ImGui::SetWindowSize({ 500,100 });

	//デモウィンドウの表示ON
	ImGui::ShowDemoWindow();

	ImGuiMan->End();

	EffectAdd();
	camera->Update();
}
void GameScene::Draw()
{


		
		Object3d::PreDraw(dxCommon_->GetCommandList());



		Object3d::PostDraw();

		/*ImGuiMan->Draw();*/

		spriteCommon->PreDraw();

		// パーティクル描画前処理
		ParticleManager::PreDraw(dxCommon_->GetCommandList());

		
		// パーティクル描画後処理
		ParticleManager::PostDraw();

		ParticleManager2::PreDraw(dxCommon_->GetCommandList());
		ParticleManager2::PostDraw();
}


void GameScene::Finalize()
{
	ImGuiMan->Finalize();
	audio->Finalize();

	delete audio;
	//ImGuiの開放
	delete ImGuiMan;



	//スプライトの開放
	delete spriteCommon;
}

void GameScene::EffectAdd()
{
}
