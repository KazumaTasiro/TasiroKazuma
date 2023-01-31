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
	assert(winApp);
	assert(input_);

	input = input_;

	winApp_ = winApp;

	dxCommon_ = dxcomon;

	audio = new Audio();
	audio->Initialize();
	audio->LoadWave("se_amd06.wav");
	

	ImGuiMan = new ImGuiManager();
	ImGuiMan->Initialize(winApp, dxCommon_);

	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon_);

	spriteCommon->LoadTexture(0, "meemu.jpg");
	spriteCommon->LoadTexture(1, "meemu.jpg");
	spriteCommon->LoadTexture(2, "wakka.jpg");

	sprite = new Sprite();
	sprite->Initialize(spriteCommon, 0);

	sprite2 = new Sprite();

	sprite2->Initialize(spriteCommon, 1);
	//sprite2->SetTextureIndex(1);
	sprite2->SetSize({ 100,100 });

	model = Model::LoadFormOBJ("cube");

	object3d = Object3d::Create();
	object3d->SetModel(model);

	object3d->Update();
	XMFLOAT2 position = sprite2->GetPosition();

	position.x += 100;

	position.y += 50;

	sprite2->SetPozition(position);

	XMFLOAT4 color = { 0,1,0,1 };

	sprite2->SetColor(color);

	sprite2->SetSize(XMFLOAT2{ 100.0f,50.0f });

	sprite3 = new Sprite();

	sprite3->Initialize(spriteCommon, 2);
	//sprite3->SetTextureIndex(2);
	sprite3->SetSize({ 100,100 });
	sprite3->SetPozition({ 0,300 });
	sprite3->SetColor({ 1,1,1,1 });
	/*sprite2->SetIsFlipY(true);*/

	posteffect = new PostEffect();

	posteffect->Initialize(dxCommon_->GetDevice());

	object3d->SetPosition({ 0,0,20 });
}
void GameScene::Update()
{
	switch (phase_)
	{
	case GameScene::Phase::TITLE:
		if (input->TriggerKey(DIK_SPACE)) {
			phase_ = Phase::GAME;
			audio->PlayWave("se_amd06.wav");
		}

		break;
	case GameScene::Phase::GAME:
		if (input->TriggerKey(DIK_SPACE)) {
			phase_ = Phase::TITLE;
			audio->PlayWave("se_amd06.wav");
		}
		break;
	}
	object3d->Update();

	char buf[10] = {};
	sprite2->SetPozition({ f[0],f[1] });

	ImGuiMan->Bigin();

	ImGui::SetWindowSize({ 500,100 });
	ImGui::SliderFloat2("position", &f[0], 0.0f, 1280.0f, "%.1f");


	//デモウィンドウの表示ON
	ImGui::ShowDemoWindow();

	ImGuiMan->End();
}
void GameScene::Draw()
{
	switch (phase_)
	{
	case GameScene::Phase::TITLE:
		spriteCommon->PreDraw();

		sprite->Draw();
		sprite2->Draw();
		sprite3->Draw();

		/*ImGuiMan->Draw();*/

		posteffect->PostDrawScene(dxCommon_->GetCommandList());

		posteffect->Draw(dxCommon_->GetCommandList());

		posteffect->PreDrawScene(dxCommon_->GetCommandList());

		/*dxCommon->ClearDepthBuffer();*/

		Object3d::PreDraw(dxCommon_->GetCommandList());

		object3d->Draw();

		Object3d::PostDraw();

		break;
	case GameScene::Phase::GAME:


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
	//3Dオブジェクトの解放
	delete object3d;
	//3Dモデル開放
	delete model;

	//スプライトの開放
	delete spriteCommon;
	delete sprite2;
}
