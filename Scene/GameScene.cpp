#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete object1;
	delete model1;
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

	ImGuiMan = new ImGuiManager();
	ImGuiMan->Initialize(winApp, dxCommon_);

	//�X�v���C�g���ʕ����̏�����
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon_);

	Object3dFbx::SetDevice(dxCommon_->GetDevice());
	Object3dFbx::SetCamera(camera);
	Object3dFbx::CreateGraphicsPipeline();
	//���f�������w�肵�ăt�@�C���ɓǂݍ���
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("cube");


	object1 = new Object3dFbx;
	object1->Initialize();
	object1->SetModel(model1);
}
void GameScene::Update()
{

	ImGuiMan->Bigin();

	ImGui::SetWindowSize({ 500,100 });

	object1->Update();
	//�f���E�B���h�E�̕\��ON
	ImGui::ShowDemoWindow();

	ImGuiMan->End();
}
void GameScene::Draw()
{



	Object3d::PreDraw(dxCommon_->GetCommandList());

	object1->Draw(dxCommon_->GetCommandList());

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
