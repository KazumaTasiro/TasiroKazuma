#include "Input/Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "FPS.h"
#include "SpriteCommon.h"
#include "Sprite.h"
#include "Object3d.h"
#include "ImGuiManager.h"
#include <imgui.h>
#include "Audio.h"
#include"GameScene.h"


//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

#pragma region 基盤システムの初期化

	//ポインタ宣言
	WinApp* winApp_ = nullptr;
	winApp_ = new WinApp;


	DirectXCommon* dxCommon_ = nullptr;
	dxCommon_ = new DirectXCommon;

	//winApp初期化
	winApp_->Initialize();

	MSG msg = {};
	//DirectX初期化処理　ここから
	dxCommon_->Initialize(winApp_);

	Input* input_ = nullptr;
	input_ = new Input;
	input_->Initalize(winApp_);

	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon_->GetDevice(), WinApp::window_width, WinApp::window_height);

	SpriteCommon* spriteCommon = nullptr;
	//スプライト共通部分の初期化
	spriteCommon = new SpriteCommon;
	spriteCommon->Initialize(dxCommon_);

	ImGuiManager* ImGuiMan = nullptr;
	ImGuiMan = new ImGuiManager();
	ImGuiMan->Initialize(winApp_, dxCommon_);
	Audio* audio = nullptr;
	audio = new Audio();
	audio->Initialize();

//	//OBJからモデルデータを読み込む
//	Model* model = Model::LoadFormOBJ("cube");
//
//	Object3d* object3d = Object3d::Create();
//	object3d->SetModel(model);
//	object3d->SetRotetion({ 30,30,30 });
//
//	object3d->Update();
//#pragma endregion 基盤システムの初期化
//
//
//	//DIrectX初期化処理ここから
//#pragma region 最初のシーンの初期化
//
//	/*Sprite* sprite = new Sprite();
//	sprite->Initialize(spriteCommon);*/
//
//	spriteCommon->LoadTexture(0, "meemu.jpg");
//	spriteCommon->LoadTexture(1, "wakka.jpg");
//
//	Sprite* sprite2 = new Sprite();
//
//	sprite2->Initialize(spriteCommon, 1);
//	sprite2->SetTextureIndex(1);
//	sprite2->SetSize({ 100,100 });
//
//	XMFLOAT2 position = sprite2->GetPosition();
//	XMFLOAT2 posE = { 0,20 };
//
//	//object3d->SetEye({posE.x,posE.y,-50 });
//	//object3d->SetTarget({ posE.x,posE.y,0 });
//	position.x += 100;
//
//	position.y += 50;
//
//	sprite2->SetPozition(position);
//
//	XMFLOAT4 color = { 0,1,0,1 };
//
//	sprite2->SetColor(color);
//
//	sprite2->SetSize(XMFLOAT2{ 100.0f,50.0f });

	/*sprite2->SetIsFlipY(true);*/




	float f[2] = { 100,100 };
#pragma endregion 最初のシーンの初期化

	//DIrectX初期化処理ここまで
	// 描画初期化処理
	// 頂点データ構造体

	FPS* fps = new FPS;


	GameScene* gameScene = nullptr;
	/* ゲームシーンの初期化*/
	gameScene = new GameScene();
	gameScene->Initialize(dxCommon_,input_);
	//ゲームループ
	while (true) {
#pragma region 基盤システムの更新
		//メッセージがある？

		//fps制限
		fps->FpsControlBegin();

		//Windowsのメッセージ処理
		if (winApp_->ProcessMessage()) {
			//ゲームループを抜ける
			break;
		}

		char buf[10] = {};
		//sprite2->SetPozition({ f[0],f[1] });

		//DirectX舞フレーム処理　ここから
		input_->Update();
		gameScene->Update();
		//object3d->Update();

		//ImGui::SetWindowSize({ 500,100 },0);

		ImGuiMan->Bigin();

		ImGui::SetWindowSize({ 500,100 });
		ImGui::SliderFloat2("position", &f[0], 0.0f, 1280.0f, "%.1f");
		//デモウィンドウの表示ON
		//ImGui::ShowDemoWindow();

		ImGuiMan->End();


#pragma endregion 基盤システムの更新

#pragma region 最初のシーンの更新

#pragma endregion 最初のシーンの更新

		dxCommon_->PreDraw();

#pragma region 最初のシーンの描画
		// 4.描画コマンドここまで

		/*sprite->Draw();*/
		//sprite2->Draw();

#pragma endregion 最初のシーンの描画

		gameScene->Draw();
		/*dxCommon->ClearDepthBuffer();*/

		//Object3d::PreDraw(dxCommon->GetCommandList());

		//object3d->Draw();

		//Object3d::PostDraw();

		//ImGuiMan->Draw();

		dxCommon_->PostDraw();
		//// 5.リソースバリアを戻す

				//FPS固定
		fps->FpsControlEnd();

	}
#pragma region 最初のシーンの終了


#pragma endregion 最初のシーンの終了

	audio->Finalize();
	ImGuiMan->Finalize();

#pragma region 基盤システムの終了
	//Audioの解放
	//delete audio;
	//ImGuiの開放
	delete ImGuiMan;
	//入力開放
	delete input_;
	////3Dオブジェクトの解放
	//delete object3d;
	////3Dモデル開放
	//delete model;

	//スプライトの開放
	delete spriteCommon;
	//delete sprite2;
	//DirectX解放
	delete dxCommon_;
	//WindowsAPIの終了処理
	winApp_->Finalize();
	//WindowsAPI解放
	delete winApp_;
#pragma endregion 基盤システムの終了
	return 0;
}