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

	//�X�v���C�g���ʕ����̏�����
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon_);

}
void GameScene::Update()
{

	ImGuiMan->Bigin();

	ImGui::SetWindowSize({ 500,100 });


	//�f���E�B���h�E�̕\��ON
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
	//ImGui�̊J��
	delete ImGuiMan;

	//3D���f���J��
	delete model;

	//�X�v���C�g�̊J��
	delete spriteCommon;

}
