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

	sprite = new Sprite();
	sprite->Initialize(spriteCommon,0);
	sprite->SetPozition({ 0,0 });
	
	sprite2 = new Sprite();
	sprite2->Initialize(spriteCommon,1);
	sprite2->SetPozition({ 300,0 });
	sprite->SetSize({ 100,100 });
	sprite2->SetSize({ 100,100 });

	model = Model::LoadFromOBJ("gorilla");
	model2 = Model::LoadFromOBJ("trakku");

	obj = Object3d::Create();
	obj2 = Object3d::Create();

	obj->SetModel(model);
	obj->wtf.position = { 0,3,10 };
	

	obj2->SetModel(model2);
	obj2->wtf.position = { 5,0,10 };

	// 3Dオブジェクト生成
	particleManager = ParticleManager::Create();
	particleManager->Update();
	particleManager->LoadTexture("meemu.jpg");

	// 3Dオブジェクト生成
	particleManager2 = ParticleManager2::Create();
	particleManager2->Update();
	particleManager2->LoadTexture("wakka.jpg");
	
}
void GameScene::Update()
{

	obj->Update();
	obj2->Update();
	ImGuiMan->Bigin();

	ImGui::SetWindowSize({ 500,100 });

	if (input->PushKey(DIK_UP)) {
		camWtf.position.y += 1;
	}
	if (input->PushKey(DIK_DOWN)) {
		camWtf.position.y -= 1;
	}
	if (input->PushKey(DIK_LEFT)) {
		camWtf.position.x -= 1;
	}
	if (input->PushKey(DIK_RIGHT)) {
		camWtf.position.x += 1;
	}
	camWtf.UpdateMat();
	camera->SetEye(camWtf.position);
	//デモウィンドウの表示ON
	ImGui::ShowDemoWindow();

	ImGuiMan->End();

	EffectAdd();
	camera->Update();
}
void GameScene::Draw()
{


		
		Object3d::PreDraw(dxCommon_->GetCommandList());
		obj->Draw();
		obj2->Draw();


		Object3d::PostDraw();

		/*ImGuiMan->Draw();*/

		spriteCommon->PreDraw();
		sprite->Draw();
		sprite2->Draw();
		// パーティクル描画前処理
		ParticleManager::PreDraw(dxCommon_->GetCommandList());
		particleManager->Draw();
		
		// パーティクル描画後処理
		ParticleManager::PostDraw();

		ParticleManager2::PreDraw(dxCommon_->GetCommandList());
		particleManager2->Draw();
		ParticleManager2::PostDraw();
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

void GameScene::EffectAdd()
{
	//パーティクル範囲
	for (int i = 0; i < 20; i++) {
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 0.01f;
		Vector3 pos{};
		pos.x += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos += obj->wtf.position;

		Vector3 pos2{};
		pos2.x += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos2.y += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos2.z += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos2 += obj2->wtf.position;
		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc = 0.00001f;
		Vector3 acc{};
		acc.x = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

		//追加
		particleManager->Add(60, pos, vel, acc, 1.0f, 0.0f);

		particleManager->Update();

		particleManager2->Add(60, pos2, vel, acc, 1.0f, 0.0f);

		particleManager2->Update();
	}
}
