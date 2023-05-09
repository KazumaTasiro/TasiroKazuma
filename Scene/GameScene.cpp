#include "GameScene.h"

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	delete camera;
	delete levelData;
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
	camWtf.Initialize();
	camWtf.position = { 0.0f, 5.0f, -15.0f };
	camera->MoveVector(camWtf.position);
	camera->SetTarget({ 0,0,0 });

	camera->Update();
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

	//レベルデータ
	levelData = JsonLoader::LoadFile("jsonTest");

	modelBox = Model::LoadFromOBJ("cube");
	modelSphere = Model::LoadFromOBJ("gorilla");

	models.insert(std::make_pair("spher", modelSphere));
	models.insert(std::make_pair("box", modelBox));

	// レベルデータからオブジェクトを生成、配置
	for (auto& objectData : levelData->objects) {
		// ファイル名から登録済みモデルを検索
		Model* model = nullptr;
		decltype(models)::iterator it = models.find(objectData.filename);
		if (it != models.end()) {
			model = it->second;
		}

		// モデルを指定して3Dオブジェクトを生成
		Object3d* newObject = Object3d::Create();
		newObject->SetModel(model);

		// 座標
		Vector3 pos;
		pos.x = objectData.translation.x;
		pos.y = objectData.translation.y;
		pos.z = objectData.translation.z;
		newObject->wtf.position = pos;

		// 回転角
		Vector3 rot;
		rot.x = objectData.rotation.x;
		rot.y = objectData.rotation.y;
		rot.z = objectData.rotation.z;
		newObject->wtf.rotation = rot;

		// 座標
		Vector3 scale;
		scale.x = objectData.scaling.x;
		scale.y = objectData.scaling.y;
		scale.z = objectData.scaling.z;
		newObject->wtf.scale = scale;

		// 配列に登録
		objects.push_back(newObject);
	}
}
void GameScene::Update()
{
	for (auto& object : objects) {
		object->Update();
	}

	ImGuiMan->Bigin();

	ImGui::SetWindowSize({ 500,100 });

	//デモウィンドウの表示ON
	ImGui::ShowDemoWindow();

	ImGuiMan->End();

	//EffectAdd();
	camera->Update();
}
void GameScene::Draw()
{



	Object3d::PreDraw(dxCommon_->GetCommandList());

	for (auto& object : objects) {
		object->Draw();
	}

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
