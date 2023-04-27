#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{

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

	camera = new Camera(winApp_->window_width, winApp_->window_height);

	ImGuiMan = new ImGuiManager();
	ImGuiMan->Initialize(winApp, dxCommon_);

	//�X�v���C�g���ʕ����̏�����
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon_);


	//���f�������w�肵�ăt�@�C���ɓǂݍ���

	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");

	Object3dF::SetDevice(dxCommon_->GetDevice());
	Object3dF::SetCamera(camera);

	Object3dF::CreateGraphicsPipeline();

	object1 = new Object3dF;
	object1->Initialize();
	object1->SetModel(model1);
	object1->PlayAnimation();

	camera->SetTarget({ 0,20,0 });
	camera->SetEye({ 0,0,100 });
	camera->Update();

}
void GameScene::Update()
{
	object1->Update();
	ImGuiMan->Bigin();

	ImGui::SetWindowSize({ 500,100 });


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
