#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete object1;
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

	ImGuiMan = new ImGuiManager();
	ImGuiMan->Initialize(winApp, dxCommon_);

	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon_);

	Object3dFbx::SetDevice(dxCommon_->GetDevice());
	Object3dFbx::SetCamera(camera);
	Object3dFbx::CreateGraphicsPipeline();
	//モデル名を指定してファイルに読み込み
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");


	object1 = new Object3dFbx;
	object1->Initialize();
	object1->SetModel(model1);
	object1->PlayAnimation();
	
}
void GameScene::Update()
{
	camera->SetEye({ -1,0,0, });
	camera->Update();
	object1->Update();

	ImGuiMan->Bigin();

	ImGui::SetWindowSize({ 500,100 });


	//デモウィンドウの表示ON
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
	//ImGuiの開放
	delete ImGuiMan;

	//3Dモデル開放
	delete model;

	//スプライトの開放
	delete spriteCommon;

}
