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
	

	ImGuiMan = new ImGuiManager();
	ImGuiMan->Initialize(winApp, dxCommon_);

	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon_);

}
void GameScene::Update()
{

	ImGuiMan->Bigin();

	ImGui::SetWindowSize({ 500,100 });


	//デモウィンドウの表示ON
	ImGui::ShowDemoWindow();

	ImGuiMan->End();
}
void GameScene::Draw()
{


		
		Object3d::PreDraw(dxCommon_->GetCommandList());


		Object3d::PostDraw();

		/*ImGuiMan->Draw();*/

		

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
