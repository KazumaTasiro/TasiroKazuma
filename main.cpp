#include "Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "FPS.h"
#include "GameScene.h"



//Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

#pragma region 基盤システムの初期化
	WinApp* winApp = nullptr;

	//WindowsAPIの初期化
	winApp = new WinApp();
	winApp->Initialize();

	//ポインタ
	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	MSG msg{};//メッセージ



	//ポインタ
	Input* input = nullptr;

	//入力の初期化
	input = new Input();
	input->Initalize(winApp);


	
	// 3Dオブジェクト静的初期化
	Object3d::StaticInitialize(dxCommon->GetDevice(), WinApp::window_width, WinApp::window_height);

	GameScene* gameScene = new GameScene();
	gameScene->Initialize(winApp, dxCommon, input);
	
#pragma endregion 基盤システムの初期化


	//DIrectX初期化処理ここから
#pragma region 最初のシーンの初期化

	
	
#pragma endregion 最初のシーンの初期化

	//DIrectX初期化処理ここまで
	// 描画初期化処理
	// 頂点データ構造体

	FPS* fps = new FPS;

	//ゲームループ
	while (true) {
#pragma region 基盤システムの更新
		//メッセージがある？

		//fps制限
		fps->FpsControlBegin();

		//Windowsのメッセージ処理
		if (winApp->ProcessMessage()) {
			//ゲームループを抜ける
			break;
		}

		

		//DirectX舞フレーム処理　ここから
		input->Update();
		gameScene->Update();

		//ImGui::SetWindowSize({ 500,100 },0);
		
		

		
#pragma endregion 基盤システムの更新

#pragma region 最初のシーンの更新

#pragma endregion 最初のシーンの更新

		dxCommon->PreDraw();

#pragma region 最初のシーンの描画
		// 4.描画コマンドここまで
#pragma endregion 最初のシーンの描画
		
		gameScene->Draw();

	/*	ImGuiMan->Draw();*/
		
		

	

		

		dxCommon->PostDraw();
		//// 5.リソースバリアを戻す

				//FPS固定
		fps->FpsControlEnd();

	}
#pragma region 最初のシーンの終了


#pragma endregion 最初のシーンの終了
	gameScene->Finalize();
#pragma region 基盤システムの終了

	//入力開放
	delete input;
	
	//DirectX解放
	delete dxCommon;
	//WindowsAPIの終了処理
	winApp->Finalize();
	//WindowsAPI解放
	delete winApp;
#pragma endregion 基盤システムの終了
	return 0;
}