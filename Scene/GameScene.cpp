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
	
	// �J��������
	camera = new Camera(WinApp::window_width, WinApp::window_height);
	camWtf.Initialize();
	camWtf.position = { 0.0f, 3.0f, -8.0f };

	ParticleManager::SetCamera(camera);
	ParticleManager2::SetCamera(camera);
	Object3d::SetCamera(camera);

	ImGuiMan = new ImGuiManager();
	ImGuiMan->Initialize(winApp, dxCommon_);

	//�X�v���C�g���ʕ����̏�����
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon_);

	spriteCommon->LoadTexture(0, "meemu.jpg");
	spriteCommon->LoadTexture(1, "wakka.jpg");




	
}
void GameScene::Update()
{


	ImGuiMan->Bigin();

	ImGui::SetWindowSize({ 500,100 });

	//�f���E�B���h�E�̕\��ON
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

		// �p�[�e�B�N���`��O����
		ParticleManager::PreDraw(dxCommon_->GetCommandList());

		
		// �p�[�e�B�N���`��㏈��
		ParticleManager::PostDraw();

		ParticleManager2::PreDraw(dxCommon_->GetCommandList());
		ParticleManager2::PostDraw();
}


void GameScene::Finalize()
{
	ImGuiMan->Finalize();
	audio->Finalize();

	delete audio;
	//ImGui�̊J��
	delete ImGuiMan;



	//�X�v���C�g�̊J��
	delete spriteCommon;
}

void GameScene::EffectAdd()
{
}
